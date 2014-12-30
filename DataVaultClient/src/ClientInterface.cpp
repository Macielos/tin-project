#include "ClientInterface.h"

ClientInterface::ClientInterface()
{

}

ClientInterface::~ClientInterface()
{

}

void ClientInterface::init(string host, short messagePort, short dataPort){
    client.init(host, messagePort, dataPort);
}

string ClientInterface::send(Message& message){
    return client.send(message);
}

void ClientInterface::sendFile(string filename){
    client.sendFile(filename);
}

void ClientInterface::receiveFile(string filename){
    client.receiveFile(filename);
}
