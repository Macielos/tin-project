#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include <iostream>
#include <sstream>
#include <string>

#include "ServerStore.h"
#include "../DataVaultAPI/src/FileTransferManager.h"
#include "../DataVaultAPI/src/Message.h"
#include "../DataVaultAPI/src/Response.h"

using namespace std;

class Server
{
        ServerStore serverStore;
        boost::asio::io_service ioService;
        FileTransferManager fileTransferManager;

        boost::mutex dataPortAccessMutex;

        int messagePort;
        int dataPort;
        int notificationPort;
        bool interrupted;

    public:
        Server(int messagePort, int dataPort, int notificationPort);
        ~Server();
        void listen();

    private:
        void handleMessage(tcp::socket* socket);
        string createResponse(Status status);
        string createResponse(Status status, string description);
        template<typename T> string serialize(T& t);
        template<typename T> void deserialize(T& t, string serializedData);
};
