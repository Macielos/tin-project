#include "ClientInterface.h"

ClientInterface::ClientInterface()
{

}

ClientInterface::~ClientInterface()
{
    //dtor
}

void ClientInterface::connect(string host, int port){
    client.connect(host, port);
}

string ClientInterface::send(string message){
    return client.send(message);
}
