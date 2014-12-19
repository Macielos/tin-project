#include "ClientInterface.h"

ClientInterface::ClientInterface()
{

}

ClientInterface::~ClientInterface()
{
    //dtor
}

void ClientInterface::init(string host, short messagePort, short dataPort){
    client.init(host, messagePort, dataPort);
}

string ClientInterface::send(Message& message){
    return client.send(message);
}
