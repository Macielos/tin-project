#include<vector>

/**
 *  Możliwe typy zdarzeń dla śledzonych plików
 */
enum EventType {FILE_MODIFIED, FILE_REMOVED, FILE_RENAMED, SHARED_FILE_MODIFIED, SHARED_FILE_REMOVED, SHARED_FILE_RENAMED, ACCESS_GRANTED, ACCESS_REVOKED};


class History
{
    vector<Event> modifiedFiles;
    vector<Event> removedFiles;
    vector<Event> renamedFiles;
    vector<Event> modifiedSharedFiles;
    vector<Event> removedSharedFiles;
    vector<Event> renamedSharedFiles;
    vector<Event> accessGrants;
    vector<Event> accessRevokes;

    public:
        History();
        ~History();

        void addEvent(EventType type, Event& event);
        vector<Event>& getEvents(EventType type);
        void clearHistory();
};
