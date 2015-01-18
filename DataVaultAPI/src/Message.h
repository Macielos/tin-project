#include <vector>
#include <string>
#include <sstream>

#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

using namespace std;

/**
 *  Działanie zlecane serwerowi przez klienta
 */
enum Action {REGISTER, LOGIN, LOGOUT, UNREGISTER, LIST, LIST_SHARED, UPLOAD, DOWNLOAD, DOWNLOAD_SHARED, REMOVE, RENAME, GIVE_ACCESS, REVOKE_ACCESS};

class Message
{
    string userId;
    string source;
    string sessionId;
    Action action;
    vector<string> parameters;

    friend class boost::serialization::access;
    template <class Archive> void serialize(Archive& ar, const unsigned int version)
    {
        ar & userId;
        ar & source;
        ar & sessionId;
        ar & action;
        ar & parameters;
    }

    public:
        Message(string userId, string source, Action action, vector<string> parameters);
        Message(Action action, vector<string> parameters);
        Message();
        ~Message();

        string getUserId();
        string getSource();
        string getSessionId();
        Action getAction();
        vector<string>& getParameters();

        void setUserId(string userId);
        void setSource(string source);
        void setSessionId(string sessionId);
        void setAction(Action action);
        void setParameters(vector<string> parameters);

        string toString();

};
