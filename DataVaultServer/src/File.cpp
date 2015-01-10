#include "File.h"

using namespace std;

File::File()
{

}

File::File(string filename)
{
    this->filename = filename;
}

File::~File()
{

}


string File::getFilename()
{
    return filename;
}

void File::setFilename(string filename)
{
    this->filename = filename;
}

int File::giveAccess(User* user)
{
    for(vector<user*>::iterator it = files.begin(); it != files.end(); ++it)
    {
        if(*it == user){
            return -2;
        }
    }
    usersWithAccess.push_back(user);
    return 0;
}

void File::revokeAccess(User* user)
{
    for(vector<user*>::iterator it = files.begin(); it != files.end(); ++it)
    {
        if(*it == user){
            usersWithAccess.erase(user);
            return 0;
        }
    return -2;
    }
}

void File::clearAccessRights()
{
    usersWithAccess.clear();
}
