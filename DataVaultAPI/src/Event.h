#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/serialization/string.hpp>

using namespace std;

namespace boost{
    namespace serialization{
        class access;
    }
}

class Event
{
        string filename;
        string username;
        string secondaryParameter;

        friend class boost::serialization::access;
        template <class Archive> void serialize(Archive& ar, const unsigned int version)
        {
            ar & filename;
            ar & username;
            ar & secondaryParameter;
        }
    public:
        Event();
        Event(string filename, string username);
        Event(string filename, string username, string secondaryParameter);
        ~Event();

        string getFilename();
        string getUsername();
        string getSecondaryParameter();

        string toString();
};
