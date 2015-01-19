#include "File.h"

using namespace std;

File::File()
{
}

File::File(string filename, string owner)
{
    this->filename = filename;
    this->owner = owner;
}

File::~File()
{

}


string File::getFilename()
{
    return filename;
}

string File::getOwner()
{
    return owner;
}

void File::setFilename(string filename)
{
    this->filename = filename;
}

void File::setOwner(string owner)
{
    this->owner = owner;
}

