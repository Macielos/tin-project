#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "ClientInterface.h"


using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 4){
        std::cerr << "Arguments: <host> <messagePort> <dataPort>" << std::endl;
        return 1;
    }

    const string host = argv[1];
    const short messagePort = atoi(argv[2]);
    const short dataPort = atoi(argv[3]);

    ClientInterface clientInterface;
    clientInterface.init(host, messagePort, dataPort);

    string userId = "xyz";
    string sender = "localhost";

 /*   Action action = UPLOAD;
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

    for(unsigned int i=0; i<parameters.size(); ++i){
        clientInterface.sendFile(parameters[i]);

    }



    clientInterface.init(host, messagePort, dataPort);*/

    Action action2 = DOWNLOAD;
    vector<string> parameters2;
    parameters2.push_back("FILE_ON_SERVER.txt");

    Message message2(userId, sender, action2, parameters2);

    string response2 = clientInterface.send(message2);
    cout<<"response2: "<<response2<<endl;

    for(unsigned int i=0; i<parameters2.size(); ++i){
        clientInterface.receiveFile(parameters2[i]);

    }
    return 0;
}
