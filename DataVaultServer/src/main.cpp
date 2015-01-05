#include "Server.h"

#include <boost/lexical_cast.hpp>

using namespace std;

int main(int argc, char* argv[])
{

    if (argc != 4){
        cerr << "Arguments: <messagePort> <dataPort> <nofiticationPort>" << endl;
        return 1;
    }

    const int messagePort = boost::lexical_cast<int>(argv[1]);
    const int dataPort = boost::lexical_cast<int>(argv[2]);
    const int notificationPort = boost::lexical_cast<int>(argv[3]);

    Server server(messagePort, dataPort, notificationPort);
    server.listen();

    return 0;
}
