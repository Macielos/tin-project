#include "File.h"
#include <string>
#include <vector>

using namespace std;

class User
{
        string username;
        string hashPass;
        map<string, File> files;

    public:
        User();
        ~User();
        User(string username);
        User(string username, string hash);

        string getUsername();
        string getHash();
        void setUsername(string username);
        void setHash(string hash);
        vector<string>& getFilelist();
        int add(string filename);
        int remove(string filename);
        int rename(string oldname, string newname);
        bool fileExists(string filename);

};
