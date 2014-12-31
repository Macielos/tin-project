#include "FileTransferManager.h"
#include "ServerStore.h"

#include <boost/archive/text_iarchive.hpp>

#include <boost/array.hpp>
#include <boost/asio.hpp>

#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include <queue>

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
        bool interrupted;

    public:
        Server(short messagePort, short dataPort);
        ~Server();
        void listen();

    private:
        void handleMessage(tcp::socket* socket);
        void waitForConfirmation();
        template<typename T> void deserialize(T& t, string serializedData);
};
