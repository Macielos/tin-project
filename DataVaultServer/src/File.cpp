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

