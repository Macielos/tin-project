#include "ClientStore.h"
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

    public:
        Client();
        ~Client();
        void init(string host, short messagePort, short dataPort);
        string send(Message& message);
        template<typename T> string serialize(T& t);
};
