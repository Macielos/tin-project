#include "ServerStore.h"

#include <boost/array.hpp>
#include <boost/asio.hpp>

using namespace std;
using boost::asio::ip::tcp;

class Server
{
        ServerStore serverStore;
        int port;

        bool interrupted;

    public:
        Server(int port);
        ~Server();
        void listen();

};
