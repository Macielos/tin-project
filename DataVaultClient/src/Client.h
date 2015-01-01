#include "ClientStore.h"
#include "../DataVaultAPI/src/FileTransferManager.h"
#include "../DataVaultAPI/src/Message.h"

#include <boost/archive/text_oarchive.hpp>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using boost::asio::ip::tcp;

class Client
{
    ClientStore clientStore;

    string host;
    short messagePort;
    short dataPort;
    short notificationPort;

    boost::asio::io_service* ioService;
    tcp::socket* socket;
    FileTransferManager* fileTransferManager;

    public:
        Client();
        ~Client();
        void init(string host, short messagePort, short dataPort, short notificationPort);
        string send(Message& message);
        void sendFile(string filename, bool notify);
        void receiveFile(string filename, bool notify);
        template<typename T> string serialize(T& t);
};
