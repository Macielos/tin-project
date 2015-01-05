#include "FileTransferManager.h"

using namespace std;

FileTransferManager::FileTransferManager(boost::asio::io_service& ioService, int dataPort, int notificationPort):
    ioService(ioService), dataPort(dataPort), notificationPort(notificationPort)
{

}

FileTransferManager::FileTransferManager(boost::asio::io_service& ioService):
    ioService(ioService)
{

}

FileTransferManager::~FileTransferManager()
{

}

/**
 *  Wyślij plik. Parameter notify każe metodzie czekać na powiadomienie od drugiej strony,
 *  że jest gotowa do odebrania pliku.
 */
bool FileTransferManager::sendFile(string destination, string filename, bool notify){
    try{
        if(notify){
            waitForNotification();
        }
        boost::array<char, 1024> buffer;
        boost::system::error_code error = boost::asio::error::host_not_found;

        tcp::resolver resolver(ioService);
        tcp::resolver::query dataQuery(destination, boost::lexical_cast<string>(dataPort));
        tcp::resolver::iterator dataEndpointIterator = resolver.resolve(dataQuery);

        boost::asio::ip::tcp::socket dataSocket(ioService);
        boost::asio::connect(dataSocket, dataEndpointIterator, error);


        ifstream sourceFile(filename.c_str(), ios_base::binary | ios_base::ate);
        if (!sourceFile){
            cout << "Nie udało się otworzyć pliku " << filename << endl;
            return false;
        }
        size_t fileSize = sourceFile.tellg();
        sourceFile.seekg(0);
        boost::asio::streambuf request;
        ostream request_stream(&request);
        request_stream << fileSize << "\n\n";
        boost::asio::write(dataSocket, request);
        while(!sourceFile.eof()){
            sourceFile.read(buffer.c_array(), (streamsize)buffer.size());
            if (sourceFile.gcount()<=0)
            {
                cerr << "Błąd odczytu pliku " << endl;
                return false;;
            }
            boost::asio::write(dataSocket, boost::asio::buffer(buffer.c_array(),
                sourceFile.gcount()),
                boost::asio::transfer_all(), error);
            if (error)
            {
                cerr << "Błąd transferu pliku:" << error << endl;
                return false;;
            }
        }
        cout << "Przesłano plik " << filename << "." << endl;
        dataSocket.close();
        return true;
    }
    catch (exception& e)
    {
        cerr << e.what() << endl;
        return false;
    }
}

/**
 *  Pobierz plik. Parameter notify każe metodzie wysłać drugiej stronie powiadomienie,
 *  że plik może być przesłany.
 */
bool FileTransferManager::receiveFile(string source, string filename, bool notify)
{
    boost::array<char, 1024> buf;
    size_t file_size = 0;
    try {
        boost::asio::ip::tcp::acceptor acceptor(ioService, boost::asio::ip::tcp::endpoint(
            boost::asio::ip::tcp::v4(), dataPort));
        boost::system::error_code error;
        boost::asio::ip::tcp::socket socket(ioService);
        if(notify){
            sendNotification();
        }
        acceptor.accept(socket);
        cout << "Rozpoczęto transfer pliku" << endl;
        boost::asio::streambuf request_buf;
        boost::asio::read_until(socket, request_buf, "\n\n");
        istream request_stream(&request_buf);
        request_stream >> file_size;
        request_stream.read(buf.c_array(), 2); // eat the "\n\n"

        ofstream output_file(filename.c_str(), ios_base::binary);
        if (!output_file)
        {
            cerr << "Nie udało się utworzyć pliku " << filename << endl;
            return false;
        }

        do
        {
            request_stream.read(buf.c_array(), (std::streamsize)buf.size());
            cout << __FUNCTION__ << " zapisała " << request_stream.gcount() << " bajtów.\n";
            output_file.write(buf.c_array(), request_stream.gcount());
        } while (request_stream.gcount()>0);

        while(true)
        {
            size_t length = socket.read_some(boost::asio::buffer(buf), error);
            if (length>0)
                output_file.write(buf.c_array(), (streamsize)length);
            if (output_file.tellp()== (fstream::pos_type)(streamsize)file_size){
                break;
            }
            if (error)
            {
                cerr << error << endl;
                return false;
            }
        }
        socket.close();
        cout << "Otrzymano " << output_file.tellp() << " bajtów."<<endl;
        return true;
    }
    catch (exception& e)
    {
        cerr << e.what() << endl;
        return false;
    }
}

/**
 *  Prześlij powiadomienie, ze kolejny plik może zostać przesłany. Podjętych zostanie
 *  maksymalnie NOTIFICATION_RETRIES prób połączenia z RETRY_INTERVAL milisekund
 *  przerwy pomiędzy kolejnymi próbami.
 */
void FileTransferManager::sendNotification(){
    tcp::resolver resolver(ioService);
    tcp::resolver::query query(host, boost::lexical_cast<string>(notificationPort));
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    tcp::socket socket(ioService);
    boost::system::error_code error;
    for(unsigned int i=0; i<NOTIFICATION_RETRIES; ++i){
        boost::asio::connect(socket, endpoint_iterator, error);
        if(!error){
            cout<<"Przesłano powiadomienie po "<<i+1<<" próbach."<<endl;
            return;
        }
        boost::this_thread::sleep(boost::posix_time::milliseconds(RETRY_INTERVAL));
    }
    cerr<<"error: "<<error<<endl;
}

/**
 *  Nasłuchuj powiadomienia z serwera, że kolejny plik może zostać przesłany.
 */
void FileTransferManager::waitForNotification(){
    cout<<"Oczekuję na powiadomienie..."<<endl;
    boost::asio::ip::tcp::acceptor acceptor(ioService, boost::asio::ip::tcp::endpoint
        (boost::asio::ip::tcp::v4(), notificationPort));
    boost::asio::ip::tcp::socket socket(ioService);
    acceptor.accept(socket);
    cout<<"Otrzymano powiadomienie."<<endl;
}

void FileTransferManager::setDataPort(int dataPort){
    this->dataPort = dataPort;
}

void FileTransferManager::setNotificationPort(int notificationPort){
    this->notificationPort = notificationPort;
}
