#include "File.h"
#include <string>
#include <vector>

using namespace std;

class User
{
        string username;
        map<string, File> files;
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

        void addEvent(EventType type, Event* event);
        History* getHistory();
        void clearHistory();
};
