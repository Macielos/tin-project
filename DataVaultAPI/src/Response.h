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
    // LOGIN | UNREGISTER
    INCORRECT_LOGIN,
    INCORRECT_PASSWORD,
    // REGISTER
    USERNAME_USED,
    // DOWNLOAD | REMOVE | RENAME
    NO_SUCH_FILE,
    ACCESS_DENIED,
    // UPLOAD
    FILE_EXISTS,
    // GIVE_ACCESS | REVOKE_ACCESS
    //    (w sumie tutaj INCORRECT_LOGIN i NO_SUCH_FILE)
    // inne
    NOT_IMPLEMENTED
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
