#include "User.h"

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
    return filemap.getFilelist();
}

int User::add(string filename)
{
    return filemap.add(filename);
}

int User::remove(string filename)
{
    return filemap.remove(filename);
}

int User::rename(string oldname, string newname)
{
    return filemap.rename(oldname, newname);
}


