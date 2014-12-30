#include "Server.h"

Server::Server(short messagePort, short dataPort):
    messagePort(messagePort), dataPort(dataPort), interrupted(false)
{
    serverStore = new ServerStore();
    ioService = new boost::asio::io_service();
    //ioService->run();
    fileTransferManager = new FileTransferManager(*ioService, dataPort);
    cout<<"Server created"<<endl;
}

Server::~Server()
{
    delete fileTransferManager;
    delete ioService;
    delete serverStore;
}

void Server::listen()
{
    try{
        tcp::acceptor acceptor(*ioService, tcp::endpoint(tcp::v4(), messagePort));
        while(!interrupted)
        {
            cout<<"Server is listening..."<<endl;
            tcp::socket* socket = new tcp::socket(*ioService);
            acceptor.accept(*socket);
            boost::thread messageHandler(boost::bind(&Server::handleMessage, this, socket));
        }
    } catch (exception& e){
        cerr << e.what() << endl;
    }
}

void Server::handleMessage(tcp::socket* socket){
    boost::array<char, 128> messageBuffer;
    boost::system::error_code error;
    //ioService->run();
    socket->read_some(boost::asio::buffer(messageBuffer), error);
    if (error){
        cerr<<"error reading message"<<endl;
    }
    cout<<"read "<<messageBuffer.data()<<endl;

    vector<string> temp;
    Message message(temp);
    deserialize(message, messageBuffer.data());

    cout<<"Message: "<<endl;
    cout<<message.toString()<<endl;

    switch(message.getAction()){
        case UPLOAD:
            dataPortAccessMutex.lock();
            socket->write_some(boost::asio::buffer("OK"), error);
            for(unsigned int i=0; i<message.getParameters().size(); ++i){
                cout<<"User "<<message.getUserId()<<" is uploading file "<<message.getParameters()[i]<<endl;
                fileTransferManager->receiveFile(message.getSource(), message.getParameters()[i]);
                cout<<"Done."<<endl;
            }
            dataPortAccessMutex.unlock();
            break;
        case DOWNLOAD:
            dataPortAccessMutex.lock();
            socket->write_some(boost::asio::buffer("OK"), error);
            for(unsigned int i=0; i<message.getParameters().size(); ++i){
                cout<<"User "<<message.getUserId()<<" is downloading file "<<message.getParameters()[i]<<endl;
                fileTransferManager->sendFile(message.getSource(), message.getParameters()[i]);
                cout<<"Done."<<endl;
            }
            dataPortAccessMutex.unlock();
            break;
        case LIST:
            cout<<"handled"<<endl;
            socket->write_some(boost::asio::buffer("OK"), error);
            break;
        default:
            cout<<"error"<<endl;
            socket->write_some(boost::asio::buffer("ERROR"), error);
    }
    delete socket;
}

/*
void Server::handleEnqueuedMessages(){
    while(true){
        queueAccessMutex.lock();
        while(messageQueue.size()==0){
            cout<<"Thread is waiting"<<endl;
            queueAccessMutex.unlock();
            queueEmpty.wait(queueEmptyMutex);
            queueAccessMutex.lock();
            cout<<"Thread is awakening"<<endl;
        }
        Message* message = messageQueue.front();

        messageQueue.pop();
        queueAccessMutex.unlock();

        switch(message->getAction()){
            case UPLOAD:
                for(unsigned int i=0; i<message->getParameters().size(); ++i){
                    cout<<"User "<<message->getUserId()<<" is uploading file "<<message->getParameters()[i]<<endl;
                    fileTransferManager->receiveFile("localhost", message->getParameters()[i]);
                    cout<<"Upload finished"<<endl;
                }
                break;
            case DOWNLOAD:
                for(unsigned int i=0; i<message->getParameters().size(); ++i){
                    cout<<"User "<<message->getUserId()<<" is downloading file "<<message->getParameters()[i]<<endl;
                    fileTransferManager->sendFile("localhost", message->getParameters()[i]);
                    cout<<"Download finished"<<endl;
                }
                break;
            default:
                cerr<<"Error: wrong message type in the queue"<<endl;
        }
    }
}*/

template<typename T> void Server::deserialize(T& t, string serializedData)
{
    try{
        cout<<"deserializing"<<endl;
        std::istringstream archive_stream(serializedData);
        boost::archive::text_iarchive archive(archive_stream);
        archive >> t;
        cout<<"deserialized"<<endl;

    }catch (std::exception& e){
        cout<<"exception"<<endl;
        boost::system::error_code error(boost::asio::error::invalid_argument);
    }
}
