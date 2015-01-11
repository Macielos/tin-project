#include "User.h"
#include <vector>
#include <string>
#include <map>

using namespace std;

class ServerStore
{
        map<string, User> users;

    public:
        ServerStore();
        ~ServerStore();

        // zwraca users.end() jak nie ma użytkownika
        vector<string> list(string username);

        /**
        zwraca 0 jak się udało
        zwraca -1 jak brak użytkownika
        zwraca -2 jak plik o nazwie istnieje
        */
        int add(string username, string filename);

        /**
        zwraca 0 jak się udało
        zwraca -1 jak brak użytkownika
        zwraca -2 jak plik o nazwie nie istnieje
        */
        int remove(string username, string filename);

        /**
        zwraca 0 jak się udało
        zwraca -1 jak brak użytkownika
        zwraca -2 jak plik o starej nazwie nie istnieje
        zwraca -3 jak plik o nowej nazwie istnieje
        */
        int rename(string username, string oldname, string newname);

        bool fileExists(string username, string filename);

        bool userExists(string username);

        /**
        zwraca 0 jak się udało
        zwraca -2 jak brak pliku
        zwraca -3 jak brak użytkownika
        zwraca -4 jak użytkownik posiada już dostęp do pliku
        zwraca -5 jak użytkownik jest właścicielem pliku
        */
        int giveAccess(string fileOwner, string requestTarget, string filename);

        /**
        zwraca 0 jak się udało
        zwraca -2 jak brak pliku
        zwraca -3 jak brak użytkownika
        zwraca -4 jak użytkownik nie posiada dostępu do pliku
        zwraca -5 jak użytkownik jest właścicielem pliku
        */
        int revokeAccess(string fileOwner, string requestTarget, string filename);

        bool hasAccess(string username, string fileOwner, string filename);

        int addEvent(string username, EventType type, Event& event);

        History* getHistory(string username);

        int clearHistory(string username);

        void clearAllHistory();

};
