#include "History.h"

History::History()
{
}

History::~History()
{
    clearHistory();
}

void History::addEvent(EventType type, Event& event)
{
    switch(type){
    case FILE_MODIFIED:
        modifiedFiles.push_back(event);
        break;
    case FILE_REMOVED:
        removedFiles.push_back(event);
        break;
    case FILE_RENAMED:
        renamedFiles.push_back(event);
        break;
    case ACCESS_GRANTED:
        accessGrants.push_back(event);
        break;
    case ACCESS_REVOKED:
        accessRevokes.push_back(event);
        break;
    }
}

vector<Event>* History::getEvents(EventType type)
{
    switch(type){
    case FILE_MODIFIED:
        return &modifiedFiles;
    case FILE_REMOVED:
        return &removedFiles;
    case FILE_RENAMED:
        return &renamedFiles;
    case ACCESS_GRANTED:
        return &accessGrants;
    case ACCESS_REVOKED:
        return &accessRevokes;
    default:
        return NULL;
    }
}

void History::clearHistory()
{
    modifiedFiles.clear();
    removedFiles.clear();
    renamedFiles.clear();
    accessGrants.clear();
    accessRevokes.clear();
}

int History::size()
{
    return modifiedFiles.size()+removedFiles.size()+renamedFiles.size()+accessGrants.size()+accessRevokes.size();
}
