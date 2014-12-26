#include "MessageHandler.h"

#include <boost/archive/text_iarchive.hpp>

#include <boost/array.hpp>
#include <boost/asio.hpp>

#include <queue>

#include <iostream>
#include <string>


using namespace std;
using boost::asio::ip::tcp;

class Server
{
        ServerStore* serverStore;
        boost::asio::io_service* ioService;
        MessageHandler* messageHandler;

        short messagePort;
        short dataPort;
        bool interrupted;

    public:
        Server(short messagePort, short dataPort);
        ~Server();
        void listen();

    private:
        template<typename T> void deserialize(T& t, string serializedData);
};
