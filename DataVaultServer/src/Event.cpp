#include "Event.h"

using namespace std;

Event::Event(long date, string filename, string username):
    date(date), filename(filename), username(username)
{
}

Event::~Event()
{
}

long Event::getDate(){
    return date;
}

string Event::getFilename(){
    return filename;
}

string Event::getUsername(){
    return username;
}
