#include "ServerStore.h"

using namespace std;

ServerStore::ServerStore()
{
    User user;
    user.setUsername("user123");
    users.insert(make_pair(user.getUsername(), user));
}

ServerStore::~ServerStore()
{

}

vector<string> ServerStore::list(string username)
{
    return users[username].getFilelist();
}

int ServerStore::add(string username, string filename)
{
    // jesli usera nie ma w bazie
    if(users.find(username) == users.end())
        return -1;
    return users[username].add(filename);
}

int ServerStore::remove(string username, string filename)
{
     // jesli usera nie ma w bazie
    if(users.find(username) == users.end())
        return -1;
    return users[username].remove(filename);
}

int ServerStore::rename(string username, string oldname, string newname)
{
    // jesli usera nie ma w bazie
    if(users.find(username) == users.end())
        return -1;
    return users[username].rename(oldname, newname);
}

bool ServerStore::fileExists(string username, string filename){
    if(users.find(username) == users.end())
        return false;
    return users[username].fileExists(filename);
}
