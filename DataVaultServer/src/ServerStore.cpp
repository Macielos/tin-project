#include "ServerStore.h"

using namespace std;

ServerStore::ServerStore()
{
    User user;
    user.setUsername("user123");
    users.insert(make_pair(user.getUsername(), user));
    User user2;
    user2.setUsername("user456");
    users.insert(make_pair(user2.getUsername(), user2));
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

bool ServerStore::userExists(string username){
    return users.find(username) != users.end();
}

int ServerStore::giveAccess(string fileOwner, string requestTarget, string filename){
    if(users.find(fileOwner) == users.end())
        return -1;
    if(users.find(requestTarget) == users.end())
        return -3;
    File* file = users[fileOwner].getFile(filename);
    if(file==NULL)
        return -2;
    return users[requestTarget].giveAccess(file);
}

int ServerStore::revokeAccess(string fileOwner, string requestTarget, string filename){
    if(users.find(fileOwner) == users.end())
        return -1;
    if(users.find(requestTarget) == users.end())
        return -3;
    File* file = users[fileOwner].getFile(filename);
    if(file==NULL)
        return -2;
    return users[requestTarget].revokeAccess(file);
}

bool ServerStore::hasAccess(string username, string fileOwner, string filename){
    if(users.find(username) == users.end())
        return false;
    if(users.find(fileOwner) == users.end())
        return false;
    File* file = users[fileOwner].getFile(filename);
    if(file==NULL)
        return false;
    return users[username].hasAccess(file);

}

int ServerStore::addEvent(string username, EventType type, Event& event){
    if(users.find(username) == users.end())
        return -3;
    users[username].addEvent(type, event);
    return 0;
}

History* ServerStore::getHistory(string username){
    if(users.find(username) == users.end())
        return NULL;
    return users[username].getHistory();
}

int ServerStore::clearHistory(string username){
    if(users.find(username) == users.end())
        return -1;
    users[username].clearHistory();
    return 0;
}

void ServerStore::clearAllHistory(){
    typedef map<string, User>::iterator it_type;
    for(it_type it = users.begin(); it != users.end(); ++it)
    {
        it->second.clearHistory();
    }
}
