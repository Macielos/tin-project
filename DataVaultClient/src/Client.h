#include "ClientStore.h"
#include "FileTransferManager.h"
#include "../DataVaultAPI/src/Message.h"

#include <boost/archive/text_oarchive.hpp>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using boost::asio::ip::tcp;

class Client
{
    ClientStore clientStore;

    string host;
    short messagePort;
    short dataPort;

    boost::asio::io_service* ioService;
    tcp::socket* socket;
    FileTransferManager* fileTransferManager;

    public:
        Client();
        ~Client();
        void init(string host, short messagePort, short dataPort);
        string send(Message& message);
        void sendFile(string filename);
        void receiveFile(string filename);
        template<typename T> string serialize(T& t);
};
