#include "ServerStore.h"

using namespace std;

ServerStore::ServerStore()
{
    User user;
    user.setUsername("user123");
    users.push_back(user);
}

ServerStore::~ServerStore()
{

}

vector<string>& ServerStore::list(string username)
{
    // Przejscie po liscie wszystkich userow
    for(vector<User>::iterator it = users.begin(); it != users.end(); ++it)
    {
        // jeśli znaleziono usera
        if(it->getUsername() == username)
            return it->getFilelist();
    }
    return noUser;
}

int ServerStore::add(string username, string filename)
{
    // Przejscie po liscie wszystkich userow
    for(vector<User>::iterator it = users.begin(); it != users.end(); ++it)
    {
        // jeśli znaleziono usera
        if(it->getUsername() == username)
            return it->add(filename);
    }
    // jesli usera nie ma w bazie
    return -1;
}

int ServerStore::remove(string username, string filename)
{
    // Przejscie po liscie wszystkich userow
    for(vector<User>::iterator it = users.begin(); it != users.end(); ++it)
    {
        // jeśli znaleziono usera
        if(it->getUsername() == username)
            return it->remove(filename);
    }
    // jesli usera nie ma w bazie
    return -1;
}

int ServerStore::rename(string username, string oldname, string newname)
{
    // Przejscie po liscie wszystkich userow
    for(vector<User>::iterator it = users.begin(); it != users.end(); ++it)
    {
        // jeśli znaleziono usera
        if(it->getUsername() == username)
            return it->rename(oldname, newname);
    }
    // jesli usera nie ma w bazie
    return -1;
}

int ServerStore::registerUser(string username, string hash)
{
    // Przejscie po liscie wszystkich userow
    for(vector<User>::iterator it = users.begin(); it != users.end(); ++it)
    {
        // jeśli znaleziono taki username
        if(it->getUsername() == username)
            return -1;
    }
    User user;
    user.setUsername(username);
    user.setHash(hash);
    users.push_back(user);
    return 0;
    // jesli username nie ma w bazie (można rejestrować)

}

bool ServerStore::fileExists(string username, string filename)
{
    for(vector<User>::iterator it = users.begin(); it != users.end(); ++it)
    {
        // jeśli znaleziono usera
        if(it->getUsername() == username)
        {
            return it->fileExists(filename);
        }
    }
    return false;
}
