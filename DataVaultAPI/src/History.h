#include <vector>
#include <boost/serialization/vector.hpp>

#include "Event.h"

using namespace std;

/**
 *  Możliwe typy zdarzeń dla śledzonych plików
 */
enum EventType {FILE_MODIFIED, FILE_REMOVED, FILE_RENAMED, ACCESS_GRANTED, ACCESS_REVOKED};


class History
{
        vector<Event> modifiedFiles;
        vector<Event> removedFiles;
        vector<Event> renamedFiles;
        vector<Event> accessGrants;
        vector<Event> accessRevokes;

        friend class boost::serialization::access;
        template <class Archive> void serialize(Archive& ar, const unsigned int version)
        {
            ar & modifiedFiles;
            ar & removedFiles;
            ar & renamedFiles;
            ar & accessGrants;
            ar & accessRevokes;
        }

    public:
        History();
        ~History();

        void addEvent(EventType type, Event& event);
        vector<Event>* getEvents(EventType type);
        void clearHistory();
        int size();
};
