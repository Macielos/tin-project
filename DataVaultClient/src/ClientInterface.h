#include "Client.h"

class ClientInterface
{
    Client client;

    public:
        ClientInterface();
        ~ClientInterface();
        void connect(string host, int port);
        string send(string message);
};
