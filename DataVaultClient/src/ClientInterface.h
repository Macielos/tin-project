#include "Client.h"

class ClientInterface
{
    Client client;

    public:
        ClientInterface();
        ~ClientInterface();
        void init(string host, short messagePort, short dataPort);
        string send(Message& message);
};
