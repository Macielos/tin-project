#include "Server.h"

/*std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}*/

const int port = 21000;

int main()
{
    Server server(port);
    server.listen();

    return 0;
}
