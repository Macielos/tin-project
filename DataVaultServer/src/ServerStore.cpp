#include "ServerStore.h"

using namespace std;

ServerStore::ServerStore()
{


}

ServerStore::~ServerStore()
{
    //dtor
}

vector<string> ServerStore::list(string username)
{
    // Przejscie po liscie wszystkich userow
    for(vector<User>::iterator it = users.begin(); it != users.end(); ++it)
    {
        // jeśli znaleziono usera
        if(it->getUsername() == username)
            return it->getFilelist();
    }
    // jesli usera nie ma w bazie
    vector<string> nouser;
    nouser.push_back("/nouser");
    return nouser;

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
