#include "ServerStore.h"
#include "../DataVaultAPI/src/Message.h"

#include <boost/archive/text_iarchive.hpp>

#include <boost/array.hpp>
#include <boost/asio.hpp>

using namespace std;
using boost::asio::ip::tcp;

class Server
{
        ServerStore serverStore;

        short messagePort;
        short dataPort;
        bool interrupted;

    public:
        Server(short messagePort, short dataPort);
        ~Server();
        void listen();
        template<typename T> void deserialize(T& t, string serializedData);
};
