/**
 *  ### Client.h ###
 *
 *      Nagłówek klasy Client.
 *
 *  Klasa Client odpowiada za zestawienie połączenia po stronie klienta. Przechowuje
 *  wszystkie potrzebne do tego informacje takie jak adres hosta czy też numery portów.
 *
 */
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

    string login;
    string password;

    boost::asio::io_service* ioService;
    tcp::socket* socket;

    public:
        Client();
        ~Client();

        void setHost(string host);
        void setMessagePort(short messagePort);
        void setDataPort(short dataPort);

        void init();
        string sendMessage(Message& message);
        template<typename T> string serialize(T& t);
};
