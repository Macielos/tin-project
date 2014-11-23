#include "ClientStore.h"

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using boost::asio::ip::tcp;

class Client
{
    ClientStore clientStore;

    tcp::socket* socket;

    public:
        Client();
        ~Client();
        void connect(string host, int port);
        string send(string message);
};
