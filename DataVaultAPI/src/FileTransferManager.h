#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>

#include <ctime>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "../DataVaultAPI/src/Message.h"

using boost::asio::ip::tcp;
using namespace std;

class FileTransferManager
{
        boost::asio::io_service& ioService;
        string host;
        short dataPort;
        short notificationPort;

    public:
        FileTransferManager(boost::asio::io_service& ioService, short dataPort, short notificationPort);
        ~FileTransferManager();
        void sendFile(string destination, string filename, bool notify);
        void receiveFile(string source, string filename, bool notify);
    private:
        void sendNotification();
        void waitForNotification();
};
