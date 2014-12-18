#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

#include "ClientInterface.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 4){
        std::cerr << "Arguments: <host> <messagePort> <dataPort>" << std::endl;
        return 1;
    }

    const string host = argv[1];
    const short messagePort = atoi(argv[2]);
    const short dataPort = atoi(argv[3]);

    ClientInterface clientInterface;
    clientInterface.init(host, messagePort, dataPort);

    string userId = "xyz";
    Action action = LIST;
    vector<string> parameters;
    parameters.push_back("filename1");

    Message message(userId, action, parameters);

    string response = clientInterface.send(message);
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
