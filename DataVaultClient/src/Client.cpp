#include "Client.h"

Client::Client()
{

}

Client::~Client()
{
    delete socket;
}

void Client::connect(string host, int port){
    cout<<"connecting"<<endl;
    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query query(host, boost::lexical_cast<std::string>(port));
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    socket = new tcp::socket(io_service);

    boost::asio::connect(*socket, endpoint_iterator);
}

string Client::send(string message){
    boost::array<char, 128> buf;
    boost::system::error_code error;

    try{
        size_t len = this->socket->read_some(boost::asio::buffer(buf), error);
        cout<<len<<endl;
        cout<<error<<endl;
        if (error){
            throw boost::system::system_error(error);
        }
    }catch (std::exception& e){
        std::cerr << e.what() << std::endl;
    }
    return string(buf.data());
}
