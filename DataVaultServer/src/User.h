#include <string>
#include <vector>

#include "File.h"
#include "History.h"

using namespace std;

class User
{
        string username;
        map<string, File> files;
        vector<File*> sharedFiles;
        History history;

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
        bool fileExists(string filename);
        File* getFile(string filename);

        int giveAccess(File* file);
        int revokeAccess(File* file);
        void clearAccessRights();
        bool hasAccess(File* file);

        void addEvent(EventType type, Event& event);
        History* getHistory();
        void clearHistory();
};
