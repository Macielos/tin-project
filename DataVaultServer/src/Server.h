#include "ServerStore.h"
#include "../DataVaultAPI/src/Message.h"

#include <boost/archive/text_iarchive.hpp>

#include <boost/array.hpp>
#include <boost/asio.hpp>

#include <deque>


using namespace std;
using boost::asio::ip::tcp;

class Server
{


        deque<Message*> messages;

        short messagePort;
        short dataPort;
        bool interrupted;

    public:
        // testy
        ServerStore serverStore;
        // koniec testow
        Server(short messagePort, short dataPort);
        ~Server();
        void listen();
        template<typename T> void deserialize(T& t, string serializedData);
};
