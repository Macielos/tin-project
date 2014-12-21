#include "ServerStore.h"
#include "../DataVaultAPI/src/Message.h"

#include <boost/archive/text_iarchive.hpp>

#include <boost/array.hpp>
#include <boost/asio.hpp>

#include <deque>

#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <fstream>
#include <sstream>

using namespace std;
using boost::asio::ip::tcp;

class Server
{
        ServerStore serverStore;

        boost::asio::io_service* ioService;

        deque<Message*> messages;

        short messagePort;
        short dataPort;
        bool interrupted;

    public:
        Server(short messagePort, short dataPort);
        ~Server();
        void init();
        void listen();
        void receiveFile(string userId, string filename);
        template<typename T> void deserialize(T& t, string serializedData);
};
