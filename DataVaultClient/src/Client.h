/**
 *  ### Client.h ###
 *
 *      Nagłówek klasy Client.
 *
 *  Klasa Client odpowiada za zestawienie połączenia po stronie klienta. Przechowuje
 *  wszystkie potrzebne do tego informacje takie jak adres hosta czy też numery portów.
 *
 */

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include "../DataVaultAPI/src/FileTransferManager.h"
#include "../DataVaultAPI/src/Message.h"
#include "../DataVaultAPI/src/LoginResponse.h"
#include "../DataVaultAPI/src/Md5.h"

using namespace std;
using boost::asio::ip::tcp;

class Client
{
        FileTransferManager fileTransferManager;

        string host;
        int messagePort;
        int dataPort;
        int notificationPort;

        string userId;
        string password;

        bool validParameters;   // flaga mówiąca czy są wprowadzone poprawne parametry połączenia
        bool connected;         // flaga mówiąca czy jest aktywne połączenie z serwerem
        bool logged;            // flaga mówiąca czy użytkownik jest akutalnie zalogowany (czy może operować na plikach)

        boost::asio::io_service ioService;
        boost::asio::ip::tcp::socket socket;
        boost::asio::ip::tcp::endpoint endpoint;


    public:
        Client();
        ~Client();

        bool setHost(string host);
        bool setMessagePort(int messagePort);
        bool setDataPort(int dataPort);
        bool setNotificationPort(int notificationPort);

        void setUserId(string userId);

        bool isValidParameters();
        bool isConnected();
        bool isLogged();

        bool connect();
        Response* sendMessage(Message& message);

        bool sendFile(string filename, bool notify);
        bool receiveFile(string filename, bool notify);
    private:
        template<typename T> string serialize(T& t);
        template<typename T> void deserialize(T& t, string serializedData);
        void checkParamCorrectness(); // sprawdza czy host, message/data/notificationPort są poprawne -> czy można próbować łączyć

};
