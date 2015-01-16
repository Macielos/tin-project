#include <vector>
#include <string>
#include <sstream>

#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

using namespace std;

/**
 *  Status wysyłany zwrotnie do klienta po obsłużeniu żądania
 */
enum Status
{
    // ogólnego przeznaczenia
    OK,
    WRONG_SYNTAX,
    NOT_IMPLEMENTED,
    // LOGIN | UNREGISTER
    INCORRECT_LOGIN,
    INCORRECT_PASSWORD,
    // REGISTER
    USERNAME_USED,
    // DOWNLOAD | REMOVE | RENAME | GIVE_ACCESS | REVOKE_ACCESS
    NO_SUCH_FILE,
    // DOWNLOAD | REMOVE | RENAME
    ACCESS_DENIED,
    // RENAME
    FILE_EXISTS,
    // GIVE_ACCESS | REVOKE_ACCESS
    NO_SUCH_USER,
    OWN_FILE,
    // GIVE_ACCESS
    ALREADY_HAVE_ACCESS,
    // REVOKE_ACCESS
    ALREADY_NO_ACCESS
};

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
        Response(Status status, string description);
        Response(Status status);
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
