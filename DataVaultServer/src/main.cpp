#include "Server.h"

#include <boost/lexical_cast.hpp>

int main(int argc, char* argv[])
{

     if (argc != 4){
        std::cerr << "Arguments: <messagePort> <dataPort> <nofiticationPort>" << std::endl;
        return 1;
    }

    const short messagePort = boost::lexical_cast<short>(argv[1]);
    const short dataPort = boost::lexical_cast<short>(argv[2]);
    const short notificationPort = boost::lexical_cast<short>(argv[3]);

    Server server(messagePort, dataPort, notificationPort);
    server.listen();

    return 0;
}
