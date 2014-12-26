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
    Action action = UPLOAD;
    vector<string> parameters;
    parameters.push_back("TEST_FILE.txt");
    parameters.push_back("TEST_FILE2.txt");

    Message message(userId, action, parameters);

    string response = clientInterface.send(message);
    cout<<"response: "<<response<<endl;

    clientInterface.sendFile("TEST_FILE.txt");
    clientInterface.sendFile("TEST_FILE2.txt");

    return 0;
}
