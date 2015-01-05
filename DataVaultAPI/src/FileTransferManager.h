#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>

#include <ctime>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Constants.h"

using boost::asio::ip::tcp;
using namespace std;

class FileTransferManager
{
        boost::asio::io_service& ioService;
        string host;
        int dataPort;
        int notificationPort;

    public:
        FileTransferManager(boost::asio::io_service& ioService, int dataPort, int notificationPort);
        FileTransferManager(boost::asio::io_service& ioService);
        ~FileTransferManager();
        bool sendFile(string destination, string filename, bool notify);
        bool receiveFile(string source, string filename, bool notify);
        void setDataPort(int dataPort);
        void setNotificationPort(int notificationPort);
    private:
        void sendNotification();
        void waitForNotification();
};
