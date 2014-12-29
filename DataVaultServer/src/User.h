#include "FileMap.h"
#include <string>
#include <vector>

using namespace std;

class User
{
    public:
        User();
        ~User();
        User(string username);
        string getUsername();
        void setUsername(string username);
        vector<string> getFilelist();
        int add(string filename);
        int remove(string filename);
        int rename(string oldname, string newname);

    private:
        string username;
        FileMap filemap;


};
