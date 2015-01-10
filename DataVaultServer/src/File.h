#include <string>
#include <map>

using namespace std;

class File
{
        string filename;
        vector<User*> usersWithAccess;

    public:
        File();
        File(string filename);
        ~File();
        string getFilename();
        void setFilename(string filename);
        int giveAccess(User* user);
        int revokeAccess(User* user);
        void clearAccessRights();

};
