#include <vector>
#include <string>
#include <sstream>

#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

using namespace std;

/**
 *  Status wysyłany zwrotnie do klienta po obsłużeniu żądania
 */
enum Status {OK, IO_ERROR, WRONG_SYNTAX, NO_SUCH_FILE, ACCESS_DENIED, NOT_IMPLEMENTED};

class Response
{
        Status status;
        string description;
        vector<string> parameters;

        friend class boost::serialization::access;
        template <class Archive> void serialize(Archive& ar, const unsigned int version)
        {
            ar & status;
            ar & description;
            ar & parameters;
        }

    public:
        Response(Status status, string description, vector<string> parameters);
        Response();
        ~Response();

        Status getStatus();
        string getDescription();
        vector<string>& getParameters();

        void setStatus(Status status);
        void setDescription(string description);
        void setParameters(vector<string> parameters);

        string toString();
};
