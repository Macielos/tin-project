#include "Server.h"

Server::Server(short messagePort, short dataPort): messagePort(messagePort), dataPort(dataPort), interrupted(false)
{
    cout<<"Server created"<<endl;
}

Server::~Server()
{
}

void Server::listen()
{
    try{
        boost::asio::io_service ioService;
        tcp::acceptor acceptor(ioService, tcp::endpoint(tcp::v4(), messagePort));

        boost::array<char, 128> messageBuffer;
        boost::system::error_code error;

        while(!interrupted)
        {
            cout<<"Server is listening..."<<endl;
            tcp::socket socket(ioService);
            acceptor.accept(socket);

            socket.read_some(boost::asio::buffer(messageBuffer), error);
            vector<string> temp;
            Message message(temp);
            deserialize(message, messageBuffer.data());

            cout<<"Reading "<<messageBuffer.data()<<endl;

            string response = "RESPONSE";

            cout<<"Writing "<<response<<endl;
            write(socket, boost::asio::buffer(response), error);
        }
    } catch (exception& e){
        cerr << e.what() << endl;
    }
}

template<typename T> void Server::deserialize(T& t, string serializedData)
{

    try{
        std::istringstream archive_stream(serializedData);
        boost::archive::text_iarchive archive(archive_stream);
        archive >> t;
    }catch (std::exception& e){
        boost::system::error_code error(boost::asio::error::invalid_argument);
    }
}
