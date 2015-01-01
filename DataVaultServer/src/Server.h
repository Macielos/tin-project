#include "../DataVaultAPI/src/FileTransferManager.h"
#include "ServerStore.h"

#include <boost/archive/text_iarchive.hpp>

#include <boost/array.hpp>
#include <boost/asio.hpp>

#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include <iostream>
#include <sstream>
#include <string>


using namespace std;
using boost::asio::ip::tcp;

class Server
{
        ServerStore* serverStore;
        boost::asio::io_service* ioService;
        FileTransferManager* fileTransferManager;

        boost::mutex dataPortAccessMutex;

        short messagePort;
        short dataPort;
        short notificationPort;
        bool interrupted;

    public:
        Server(short messagePort, short dataPort, short notificationPort);
        ~Server();
        void listen();

    private:
        void handleMessage(tcp::socket* socket);
        template<typename T> void deserialize(T& t, string serializedData);
};
