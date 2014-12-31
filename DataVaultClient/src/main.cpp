#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "ClientInterface.h"


using namespace std;

ClientInterface clientInterface;

void sendMessage(Message* message);

int main(int argc, char* argv[])
{
    if (argc != 4){
        std::cerr << "Arguments: <host> <messagePort> <dataPort>" << std::endl;
        return 1;
    }

    const string host = argv[1];
    const short messagePort = atoi(argv[2]);
    const short dataPort = atoi(argv[3]);


    clientInterface.init(host, messagePort, dataPort);

    string userId = "xyz";
    string sender = "localhost";

    Action action = UPLOAD;
    vector<string> parameters;
    parameters.push_back("TEST_FILE.txt");
    parameters.push_back("TEST_FILE2.txt");
    parameters.push_back("TEST_FILE3.txt");
    parameters.push_back("Firefly Soundtrack - River's Perception.wma");
    parameters.push_back("Starcraft 2 Soundtrack HQ all Terran Themes.mp3");
    parameters.push_back("The Witcher 3_ Wild Hunt - The Trail - Trailer Music.mp3");
    parameters.push_back("The Witcher Soundtrack - Peaceful Moments.mp3");

    Message message(userId, sender, action, parameters);

    string response = clientInterface.send(message);
    cout<<"response: "<<response<<endl;

  //  char a;
    for(unsigned int i=0; i<parameters.size(); ++i){
        clientInterface.sendFile(parameters[i]);
       // cin >> a;
    }



/*    clientInterface.init(host, messagePort, dataPort);

    Action action2 = DOWNLOAD;
    vector<string> parameters2;
    parameters2.push_back("FILE_ON_SERVER.txt");
    parameters2.push_back("FILE_ON_SERVER2.txt");
    parameters2.push_back("FILE_ON_SERVER3.txt");
    parameters2.push_back("Starcraft 2 Soundtrack HQ all Terran Themes.mp3");
    parameters2.push_back("The Witcher Soundtrack - Peaceful Moments.mp3");

    Message message2(userId, sender, action2, parameters2);

    boost::thread messageSender (sendMessage, &message2);

    for(unsigned int i=0; i<parameters2.size(); ++i){
        clientInterface.receiveFile(parameters2[i], i!=0);
    }
    messageSender.join();*/
    return 0;
}

void sendMessage(Message* message){
    cout<<"sending message: "<<endl<<message->toString()<<endl;
    string response = clientInterface.send(*message);
    cout<<"response for msg: "<<response<<endl;
}
