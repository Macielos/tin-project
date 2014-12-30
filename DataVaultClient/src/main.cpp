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
    Action action = UPLOAD;

    vector<string> parameters;
    parameters.push_back("TEST_FILE.txt");

    Message message(userId, sender, action, parameters);

    string response = clientInterface.send(message);
    cout<<"response: "<<response<<endl;
    clientInterface.sendFile("TEST_FILE.txt");

    clientInterface.init(host, messagePort, dataPort);

    vector<string> parameters2;
    parameters2.push_back("TEST_FILE2.txt");

    Message message2(userId, sender, action, parameters2);

    string response2 = clientInterface.send(message2);
    cout<<"response2: "<<response2<<endl;
    clientInterface.sendFile("TEST_FILE2.txt");

    return 0;
}
