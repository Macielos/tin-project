#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace std;

class Event
{
        long date;
        string filename;
        string username;
    public:
        Event(long date, string filename, string username);
        ~Event();

        long getDate();
        string getFilename();
        string getUsername();
};
