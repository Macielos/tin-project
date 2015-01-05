#include "Server.h"

Server::Server(int messagePort, int dataPort, int notificationPort):
    fileTransferManager(ioService, dataPort, notificationPort), messagePort(messagePort), dataPort(dataPort), notificationPort(notificationPort), interrupted(false)
{
    cout<<"Serwer uruchomiony"<<endl;
}

Server::~Server()
{
}

/**
 *  Główna pętla serwera. Serwer bedzie nasłuchiwal przychodzących połączeń. Dla nowego połączenia
 *  zostanie uruchomiony nowy wątek, który je obsłuży przy użyciu osobnego gniazda.
 */
void Server::listen()
{
    try{
        tcp::acceptor acceptor(ioService, tcp::endpoint(tcp::v4(), messagePort));
        while(!interrupted)
        {
            cout<<"Serwer nasłuchuje..."<<endl;
            tcp::socket* socket = new tcp::socket(ioService);
            acceptor.accept(*socket);
            boost::thread messageHandler(boost::bind(&Server::handleMessage, this, socket));
        }
    } catch (exception& e){
        cerr << e.what() << endl;
    }
}

/**
 *  Wątek obsługujący pojedynczą wiadomość przesłaną do serwera.
 */
void Server::handleMessage(tcp::socket* socket){
    boost::array<char, 2048> messageBuffer;
    boost::system::error_code error;
    ioService.run();
    socket->read_some(boost::asio::buffer(messageBuffer), error);
    if (error){
        cerr<<error<<endl;
        socket->close();
        delete socket;
        return;
    }

    Message message;
    deserialize(message, messageBuffer.data());

    cout<<"Otrzymano wiadomość: "<<endl;
    cout<<message.toString()<<endl;

    bool result;
    switch(message.getAction()){
        case UPLOAD:
            dataPortAccessMutex.lock();
            socket->write_some(boost::asio::buffer(createResponse(OK)), error);
            for(unsigned int i=0; i<message.getParameters().size(); ++i){
                cout<<"Użytkownik "<<message.getUserId()<<" przesyła plik "<<message.getParameters()[i]<<endl;
                result = fileTransferManager.receiveFile(message.getSource(), message.getParameters()[i], i!=0);
                if(!result){
                    cerr<<"Nie przesłano pliku"<<endl;
                    break;
                }
                cout<<"Gotowe."<<endl;
            }
            dataPortAccessMutex.unlock();
            break;
        case DOWNLOAD:
            dataPortAccessMutex.lock();
            socket->write_some(boost::asio::buffer(createResponse(OK)), error);
            for(unsigned int i=0; i<message.getParameters().size(); ++i){
                cout<<"Użytkownik "<<message.getUserId()<<" pobiera plik "<<message.getParameters()[i]<<endl;
                result = fileTransferManager.sendFile(message.getSource(), message.getParameters()[i], true);
                if(!result){
                    cerr<<"Nie przesłano pliku"<<endl;
                    break;
                }
                cout<<"Gotowe."<<endl;
            }
            dataPortAccessMutex.unlock();
            break;
        default:
            socket->write_some(boost::asio::buffer(createResponse(NOT_IMPLEMENTED)), error);
    }
    socket->close();
    delete socket;
}

string Server::createResponse(Status status){
    return createResponse(status, "");
}

string Server::createResponse(Status status, string description){
    vector<string> temp;
    Response response(status, description, temp);
    return serialize(response);
}

template<typename T> string Server::serialize(T& t)
{
    std::ostringstream archive_stream;
    boost::archive::text_oarchive archive(archive_stream);
    archive << t;
    return archive_stream.str();
}

template<typename T> void Server::deserialize(T& t, string serializedData)
{
    std::istringstream archive_stream(serializedData);
    boost::archive::text_iarchive archive(archive_stream);
    archive >> t;
}
