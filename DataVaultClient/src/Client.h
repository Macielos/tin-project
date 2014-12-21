#include "ClientStore.h"
#include "../DataVaultAPI/src/Message.h"

#include <boost/archive/text_oarchive.hpp>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>
#include <boost/asio.hpp>
#include <fstream>
#include <sstream>

using namespace std;
using boost::asio::ip::tcp;

class Client
{
    ClientStore clientStore;

    string host;
    short messagePort;
    short dataPort;

    boost::asio::io_service* ioService;
    tcp::socket* messageSocket;
    tcp::socket* dataSocket;

    public:
        Client();
        ~Client();
        void init(string host, short messagePort, short dataPort);
        string send(Message& message);
        void sendFile(string fileName);
        template<typename T> string serialize(T& t);
};
