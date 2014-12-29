#include "FileMap.h"

using namespace std;

FileMap::FileMap()
{

}

FileMap::~FileMap()
{

}

vector<string> FileMap::getFilelist()
{
    return files;
}

int FileMap::add(string filename)
{
    for(vector<string>::iterator it = files.begin(); it != files.end(); ++it)
    {
        // jeśli znaleziono plik
        if(*it == filename)
            return -2;
    }
    // jesli nie ma takiego pliku
    files.push_back(filename);
    return 0;
}

int FileMap::remove(string filename)
{
    for(vector<string>::iterator it = files.begin(); it != files.end(); ++it)
    {
        // jeśli znaleziono plik
        if(*it == filename)
        {
             files.erase(it);
             return 0;
        }
    }
    // jesli nie ma takiego pliku
    return -2;
}

int FileMap::rename(string oldname, string newname)
{
    bool existsNewname;
    for(vector<string>::iterator it = files.begin(); it != files.end(); ++it)
    {
        // jeśli istnieje plik o nowej nazwie
        if(*it == newname)
            existsNewname = true;
    }

    for(vector<string>::iterator it = files.begin(); it != files.end(); ++it)
    {
        // jeśli znaleziono plik o starej nazwie
        if(*it == oldname)
        {
            // jeśli nie istnieje plik o nowej nazwie
            if(existsNewname == false)
            {
                *it = newname;
                return 0;
            }
            // jeśli już istnieje plik o nowej nazwie
            else
                return -3;
        }
    }
    // jesli nie ma takiego pliku
    return -2;
}
