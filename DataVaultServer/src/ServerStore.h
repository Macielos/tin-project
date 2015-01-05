#include <string>

using namespace std;

class ServerStore
{
    public:
        ServerStore();
        ~ServerStore();

        string list(string username);
        int save(string username, string filename);
        bool exists(string username, string filename);
        int remove(string username, string filename);
        int rename(string username, string oldname, string newname);

};
