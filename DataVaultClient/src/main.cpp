#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

#include "ClientInterface.h"

using namespace std;

int main(int argc, char* argv[])
{
    std::cout<<argc<<std::endl;
    if (argc != 3){
        std::cerr << "Arguments: <host> <port>" << std::endl;
        return 1;
    }

    const string host = argv[1];
    const int port = atoi(argv[2]);

    ClientInterface clientInterface;
    clientInterface.connect(host, port);

    string response = clientInterface.send("Hello");
    cout<<"response: "<<response<<endl;

    return 0;
}


/*

  try
  {


    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query query(argv[1], "21000");
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    tcp::socket socket(io_service);

    std::cout<<"trying to connect..."<<std::endl;

    boost::asio::connect(socket, endpoint_iterator);

    std::cout<<"connected"<<std::endl;

    for (;;)
    {
      boost::array<char, 128> buf;
      boost::system::error_code error;

      size_t len = socket.read_some(boost::asio::buffer(buf), error);

      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.

      std::cout.write(buf.data(), len);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
*/
