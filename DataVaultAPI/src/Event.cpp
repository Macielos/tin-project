#include "Event.h"

using namespace std;

Event::Event()
{
}

Event::Event(string filename, string username):
    filename(filename), username(username), secondaryParameter("")
{
}

Event::Event(string filename, string username, string secondaryParameter):
    filename(filename), username(username), secondaryParameter(secondaryParameter)
{
}

Event::~Event()
{
}

string Event::getFilename(){
    return filename;
}

string Event::getUsername(){
    return username;
}

string Event::getSecondaryParameter(){
    return secondaryParameter;
}

string Event::toString(){
    stringstream ss;
    ss << "username = "<<username<<endl;
    ss << "filename = "<<filename<<endl;
    ss << "secondaryParameter = "<<secondaryParameter<<endl;
    ss << endl;
    return ss.str();
}
