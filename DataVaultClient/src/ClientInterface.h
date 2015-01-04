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

using std::vector;

enum ConnectionParameter {HOST, MESSAGE_PORT, DATA_PORT};

class ClientInterface
{
    private:
        Client client;          // klient do zestawienia połączenia z serwerem
        vector<string> command; // polecenie (komenda) wpisana przez użytkownika podzielona na wyrazy

        void splitCommandToWords(string commandLine);
        bool interpretCommand(string commandLine);
        void followTaskOnServer(Action action);
        string sendMessage(Message& message);
        void showHelp();
        void connect();

    public:
        ClientInterface();
        ~ClientInterface();

        void changeParameter(ConnectionParameter param, string value);
        bool getCommand();
};
