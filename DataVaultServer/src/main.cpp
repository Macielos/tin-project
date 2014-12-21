#include "Server.h"

#include <boost/lexical_cast.hpp>

int main(int argc, char* argv[])
{

     if (argc != 3){
        std::cerr << "Arguments: <messagePort> <dataPort>" << std::endl;
        return 1;
    }

    const short messagePort = boost::lexical_cast<short>(argv[1]);
    const short dataPort = boost::lexical_cast<short>(argv[2]);

    Server server(messagePort, dataPort);
    server.init();
    server.listen();

    return 0;
}
