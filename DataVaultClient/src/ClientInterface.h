/**
 *  ### ClientInterface.h ###
 *
 *      Nagłówek klasy ClientInterface.
 *
 *  Klasa ClientInterface odpowiada za interakcje z użytkownikiem.
 *
 */
#include "Client.h"
#include <algorithm>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

using std::vector;

enum ConnectionParameter {HOST, MESSAGE_PORT, DATA_PORT, NOTIFICATION_PORT};

class ClientInterface
{
        Client client;          // klient do zestawienia połączenia z serwerem
        vector<string> command; // polecenie (komenda) wpisana przez użytkownika podzielona na wyrazy
    public:
        ClientInterface();
        ~ClientInterface();
        void changeParameter(ConnectionParameter param, string value);
        bool getCommand();
    private:
        void splitCommandToWords(string commandLine);
        inline bool checkFileExist (const std::string& name);
        unsigned int checkFilenamesCorectness();
        bool interpretCommand(string commandLine);
        void followTaskOnServer(Action action);
        void processResponseUPLOAD(Response* response, Message& message);
        void processResponseDOWNLOAD(Response* response, Message& message);
        inline void showHelp();
        void connect();
};
