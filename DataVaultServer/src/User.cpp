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

User::User(string username, string hash)
{
    this->username = username;
    this->hashPass = hash;
}

string User::getUsername()
{
    return this->username;
}

string User::getHash()
{
    return this->hashPass;
}

void User::setUsername(string username)
{
    this->username = username;
}

void User::setHash(string hash)
{
    this->hashPass = hashPass;
}

vector<string>& User::getFilelist()
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

bool User::fileExists(string filename)
{
    return filemap.exists(filename);
}
