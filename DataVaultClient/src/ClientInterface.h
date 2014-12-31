#include "Client.h"

class ClientInterface
{
    Client client;

    public:
        ClientInterface();
        ~ClientInterface();
        void init(string host, short messagePort, short dataPort);
        string send(Message& message);
        void sendFile(string filename);
        void receiveFile(string filename, bool confirm);
};
