#include "Client.h"

Client::Client()
{

}

Client::~Client()
{
    delete socket;
    delete ioService;
}

/**
 *  Nawiąż połączenie z serwerem i przygotuj komponenty potrzebne do komunikacji oraz transferu plikow
 */
void Client::init(string host, short messagePort, short dataPort, short notificationPort){
    this->host = host;
    this->messagePort = messagePort;
    this->dataPort = dataPort;
    this->notificationPort = notificationPort;

    cout<<"connecting"<<endl;
    ioService = new boost::asio::io_service();

    tcp::resolver resolver(*ioService);
    tcp::resolver::query query(host, boost::lexical_cast<string>(messagePort));
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    socket = new tcp::socket(*ioService);

    boost::asio::connect(*socket, endpoint_iterator);

    fileTransferManager = new FileTransferManager(*ioService, dataPort, notificationPort);
}

/**
 *  Wyślij wiadomość do serwera i czekaj na odpowiedź
 */
string Client::send(Message& message){
    boost::array<char, 2048> messageBuffer;
    boost::system::error_code error;

    try{
        cout<<"Writing message of "<<message.getUserId()<<endl;
        string serializedMessage = serialize(message);
        write(*socket, boost::asio::buffer(serializedMessage), error);
        socket->read_some(boost::asio::buffer(messageBuffer), error);
        cout<<"Reading "<<messageBuffer.data()<<endl;
        if (error){
            throw boost::system::system_error(error);
        }
    }catch (std::exception& e){
        std::cerr << e.what() << std::endl;
    }
    return string(messageBuffer.data());
}

void Client::sendFile(string filename, bool notify){
    fileTransferManager->sendFile(host, filename, notify);
}

void Client::receiveFile(string filename, bool notify){
    fileTransferManager->receiveFile(host, filename, notify);
}

template<typename T> string Client::serialize(T& t){
    std::ostringstream archive_stream;
    boost::archive::text_oarchive archive(archive_stream);
    archive << t;
    return archive_stream.str();
}
