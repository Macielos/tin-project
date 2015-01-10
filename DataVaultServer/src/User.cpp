#include "User.h"

using namespace std;

User::User()
{

}

User::~User()
{

}

User::User(string username)
{
    this->username = username;
}

string User::getUsername()
{
    return this->username;
}

void User::setUsername(string username)
{
    this->username = username;
}

vector<string> User::getFilelist()
{
    vector<string> fileList;
    typedef map<string, File>::iterator it_type;
    for(it_type it = files.begin(); it != files.end(); ++it)
    {
        fileList.push_back(it->second.getFilename());
    }
    return fileList;
}

int User::add(string filename)
{
    // jesli nie ma takiego pliku
    if(files.find(filename) != files.end())
        return -2;
    File file = File(filename);
    files.insert(make_pair(filename, file));
    return 0;
}

int User::remove(string filename)
{
    // jesli nie ma takiego pliku
    if(files.find(filename) == files.end())
        return -2;
    files.erase(filename);
    return 0;
}

int User::rename(string oldname, string newname)
{
    // jeśli nie istnieje plik o starej nazwie
    if(files.find(oldname) == files.end())
        return -2;

    // jeśli istnieje plik o nowej nazwie
    if(files.find(newname) != files.end())
        return -3;

    // zmiana nazwy
    File file = File();
    files.insert(make_pair(newname, file));
    files[newname] = files[oldname];
    files[newname].setFilename(newname);
    files.erase(oldname);
    return 0;
}

bool User::fileExists(string filename){
    return files.find(filename) != files.end();
}

void User::addEvent(EventType type, Event* event){
    history.addEvent(type, event);
}

History* User::getHistory(){
    return &history;
}
void User::clearHistory(){
    history.clearHistory();
}
