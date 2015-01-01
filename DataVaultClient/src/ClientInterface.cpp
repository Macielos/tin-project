#include "ClientInterface.h"

ClientInterface::ClientInterface()
{

}

ClientInterface::~ClientInterface()
{

}

void ClientInterface::init(string host, short messagePort, short dataPort, short notificationPort){
    client.init(host, messagePort, dataPort, notificationPort);
}

string ClientInterface::send(Message& message){
    return client.send(message);
}

void ClientInterface::sendFile(string filename, bool confirm){
    client.sendFile(filename, confirm);
}

void ClientInterface::receiveFile(string filename, bool confirm){
    client.receiveFile(filename, confirm);
}
