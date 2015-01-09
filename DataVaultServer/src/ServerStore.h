#include "User.h"
#include <vector>
#include <string>
#include <map>

using namespace std;

class ServerStore
{
    public:
        ServerStore();
        ~ServerStore();
        // zwraca users.end() jak nie ma usera
        vector<string> list(string username);
        /*
        zwraca 0 jak się udało
        zwraca -1 jak brak usera
        zwraca -2 jak plik o nazwie istnieje
        */
        int add(string username, string filename);
        /*
        zwraca 0 jak się udało
        zwraca -1 jak brak usera
        zwraca -2 jak plik o nazwie nie istnieje
        */
        int remove(string username, string filename);
        /*
        zwraca 0 jak się udało
        zwraca -1 jak brak usera
        zwraca -2 jak plik o starej nazwie nie istnieje
        zwraca -3 jak plik o nowej nazwie istnieje
        */
        int rename(string username, string oldname, string newname);

        // bedzie laczone pewnie z klasa michala
        // vector<User> users;
        map<string, User> users;


    private:

};
