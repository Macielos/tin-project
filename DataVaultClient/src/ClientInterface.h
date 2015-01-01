#include "Client.h"

class ClientInterface
{
    Client client;

    public:
        ClientInterface();
        ~ClientInterface();
        void init(string host, short messagePort, short dataPort, short notificationPort);
        string send(Message& message);
        void sendFile(string filename, bool notify);
        void receiveFile(string filename, bool notify);
};
