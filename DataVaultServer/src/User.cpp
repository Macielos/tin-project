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

File* User::getFile(string filename){
    if(files.find(filename) == files.end()){
        return NULL;
    }
    return &files[filename];
}

int User::giveAccess(File* file)
{
    if(fileExists(file->getFilename())){
        return -5;
    }
    for(vector<File*>::iterator it = sharedFiles.begin(); it != sharedFiles.end(); ++it)
    {
        if(*it == file){
            return -4;
        }
    }
    sharedFiles.push_back(file);
    return 0;
}

int User::revokeAccess(File* file)
{
    if(fileExists(file->getFilename())){
        return -5;
    }
    for(vector<File*>::iterator it = sharedFiles.begin(); it != sharedFiles.end(); ++it)
    {
        if(*it == file){
            sharedFiles.erase(it);
            return 0;
        }
    }
    return -4;
}

void User::clearAccessRights()
{
    sharedFiles.clear();
}

bool User::hasAccess(File* file){
    for(vector<File*>::iterator it = sharedFiles.begin(); it != sharedFiles.end(); ++it)
    {
        if(*it == file){
            return true;
        }
    }
    return false;
}

void User::addEvent(EventType type, Event& event){
    history.addEvent(type, event);
}

History* User::getHistory(){
    return &history;
}
void User::clearHistory(){
    history.clearHistory();
}


