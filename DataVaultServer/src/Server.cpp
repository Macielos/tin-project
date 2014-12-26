#include "Server.h"

Server::Server(short messagePort, short dataPort):
    messagePort(messagePort), dataPort(dataPort), interrupted(false)
{
    serverStore = new ServerStore();
    ioService = new boost::asio::io_service();
    messageHandler = new MessageHandler(*serverStore, *ioService, dataPort);
    messageHandler->run();
    cout<<"Server created"<<endl;
}

Server::~Server()
{
    delete messageHandler;
    delete ioService;
    delete serverStore;
}

void Server::listen()
{
    try{
        tcp::acceptor acceptor(*ioService, tcp::endpoint(tcp::v4(), messagePort));

        boost::array<char, 128> messageBuffer;
        boost::system::error_code error;

        while(!interrupted)
        {
            cout<<"Server is listening..."<<endl;
            tcp::socket socket(*ioService);
            acceptor.accept(socket);

            socket.read_some(boost::asio::buffer(messageBuffer), error);
            vector<string> temp;
            Message message(temp);
            cout<<"Message buffer: "<<messageBuffer.data()<<endl;
            deserialize(message, messageBuffer.data());

            cout<<"Adding message: "<<endl;
            cout<<message.toString()<<endl;

            messageHandler->add(&message);
            cout<<"Messages: "<<messageHandler->getMessageQueueSize()<<endl;
            cout<<messageHandler->messageQueue.front()->toString()<<endl;

            write(socket, boost::asio::buffer("response"), error);

           // messageHandler->handleMessageThread();

        }
    } catch (exception& e){
        cerr << e.what() << endl;
    }
}

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
