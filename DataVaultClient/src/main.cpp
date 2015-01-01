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
    if (argc != 5){
        cerr << "Arguments: <host> <messagePort> <dataPort> <notificationPort>" << endl;
        return 1;
    }

    const string host = argv[1];
    const short messagePort = atoi(argv[2]);
    const short dataPort = atoi(argv[3]);
    const short notificationPort = atoi(argv[4]);

   clientInterface.init(host, messagePort, dataPort, notificationPort);

    string userId = "xyz";
    string sender = "localhost";

    Action action = UPLOAD;
    vector<string> parameters;
    parameters.push_back("TEST_FILE.txt");
    parameters.push_back("TEST_FILE2.txt");
    parameters.push_back("TEST_FILE3.txt");

    Message message(userId, sender, action, parameters);

    string response = clientInterface.send(message);
    cout<<"response for msg: "<<response<<endl;

    for(unsigned int i=0; i<parameters.size(); ++i){
        clientInterface.sendFile(parameters[i], i!=0);
    }

    boost::this_thread::sleep(boost::posix_time::seconds(2));
    clientInterface.init(host, messagePort, dataPort, notificationPort);

    Action action2 = DOWNLOAD;
    vector<string> parameters2;
    parameters2.push_back("FILE_ON_SERVER.txt");
    parameters2.push_back("FILE_ON_SERVER2.txt");
    parameters2.push_back("FILE_ON_SERVER3.txt");
    Message message2(userId, sender, action2, parameters2);

    string response2 = clientInterface.send(message2);
    cout<<"response for msg2: "<<response2<<endl;

    for(unsigned int i=0; i<parameters2.size(); ++i){
        clientInterface.receiveFile(parameters2[i], true);
    }
    return 0;
}
