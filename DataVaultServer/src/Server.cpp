#include "Server.h"

Server::Server(int port): port(port), interrupted(false)
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
        tcp::acceptor acceptor(ioService, tcp::endpoint(tcp::v4(), port));

        while(!interrupted)
        {
            cout<<"Server is listening..."<<endl;
            tcp::socket socket(ioService);
            acceptor.accept(socket);

            string message = "RESPONSE";

            boost::system::error_code ignored_error;
            write(socket, boost::asio::buffer(message), ignored_error);
        }
    } catch (exception& e){
        cerr << e.what() << endl;
    }
}
