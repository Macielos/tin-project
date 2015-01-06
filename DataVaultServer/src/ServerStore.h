#include "User.h"
#include <vector>
#include <string>

using namespace std;

class ServerStore
{
        // bedzie laczone pewnie z klasa michala
        vector<User> users;
        vector<string> noUser;

    public:
        ServerStore();
        ~ServerStore();

        vector<string>& list(string username);

        int save(string username, string filename);

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

        /*
        zwraca 0 jak się udało
        zwraca -1 jak username w użyciu
        */
        int registerUser(string username, string hash);

        /*
        zwraca 0 jak się udało
        zwraca -1 jak brak usera
        zwraca -2 jak błędny hash
        */
        int unregisterUser(string username, string hash);

        bool fileExists(string username, string filename);
};
