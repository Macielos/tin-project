#include <string>
#include <vector>

#include "File.h"
#include "../DataVaultAPI/src/LoginResponse.h"

using namespace std;

class User
{
        string username;
        string passwordHash;

        map<string, File> files;
        vector<File*> sharedFiles;
        History history;

    public:
        User();
        ~User();
        User(string username);
        User(string username, string hash);

        string getUsername();
        string getPasswordHash();
        void setUsername(string username);
        void setPasswordHash(string passwordHash);

        vector<string> getFilelist();
        int add(string filename);
        int remove(string filename);
        int rename(string oldname, string newname);
        bool fileExists(string filename);
        File* getFile(string filename);
        void deleteFiles();

        int giveAccess(File* file);
        int revokeAccess(File* file);
        void clearAccessRights();
        bool hasAccess(File* file);

        void addEvent(EventType type, Event& event);
        History* getHistory();
        void clearHistory();
};
