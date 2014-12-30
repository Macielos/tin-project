#include <boost/asio.hpp>
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

    public:
        FileTransferManager(boost::asio::io_service& ioService, short dataPort);
        ~FileTransferManager();
        void sendFile(string destination, string filename);
        void receiveFile(string destination, string filename);
};
