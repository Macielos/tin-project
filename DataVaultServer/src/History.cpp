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
        modifiedFiles.add(event);
        break;
    case FILE_REMOVED:
        removedFiles.add(event);
        break;
    case FILE_RENAMED:
        renamedFiles.add(event);
        break;
    case SHARED_FILE_MODIFIED:
        modifiedSharedFiles.add(event);
        break;
    case SHARED_FILE_REMOVED:
        removedSharedFiles.add(event);
        break;
    case SHARED_FILE_RENAMED:
        renamedSharedFiles.add(event);
        break;
    case ACCESS_GRANTED:
        accessGrants.add(event);
        break;
    case ACCESS_REVOKED:
        accessRevokes.add(event);
        break;
    }
}

vector<Event>& History::getEvents(EventType type)
{
    switch(type){
    case FILE_MODIFIED:
        return modifiedFiles;
    case FILE_REMOVED:
        return removedFiles;
    case FILE_RENAMED:
        return renamedFiles;
    case SHARED_FILE_MODIFIED:
        return modifiedSharedFiles;
    case SHARED_FILE_REMOVED:
        return removedSharedFiles;
    case SHARED_FILE_RENAMED:
        return renamedSharedFiles;
    case ACCESS_GRANTED:
        return accessGrants;
    case ACCESS_REVOKED:
        return accessRevokes;
    }
}

void History::clearHistory()
{
    modifiedFiles.clear();
    removedFiles.clear();
    renamedFiles.clear();
    modifiedSharedFiles.clear();
    removedSharedFiles.clear();
    renamedSharedFiles.clear();
    accessGrants.clear();
    vaccessRevokes.clear();
}
