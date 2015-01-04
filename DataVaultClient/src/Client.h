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
#include <boost/exception/diagnostic_information.hpp>

using namespace std;
using boost::asio::ip::tcp;

class Client
{
    private:
        ClientStore clientStore;

        string host;
        unsigned short messagePort;
        unsigned short dataPort;
        string userId;
        string password;

        bool validParameters;   // flaga mówiąca czy są wprowadzone poprawne parametry połączenia
        bool connected;         // flaga mówiąca czy jest aktywne połączenie z serwerem
        bool logged;            // flaga mówiąca czy użytkownik jest akutalnie zalogowany (czy może operować na plikach)

        boost::asio::io_service ioService;
        boost::asio::ip::tcp::socket socket;
        boost::asio::ip::tcp::endpoint endpoint;

        void checkParamCorrectness(); // sprawdza czy host, message/dataPort są poprawne -> czy można próbować łączyć

    public:
        Client();
        ~Client();

        bool setHost(string host);
        bool setMessagePort(int messagePort);
        bool setDataPort(int dataPort);

        bool isValidParameters();
        bool isConnected();
        bool isLogged();

        bool connect();
        string sendMessage(Message& message);
        template<typename T> string serialize(T& t);
};
