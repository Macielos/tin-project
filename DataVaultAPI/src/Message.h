#include <vector>
#include <string>
#include <sstream>

#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

using namespace std;
using namespace boost::archive;

enum Action {REGISTER, LOGIN, LOGOUT, UNREGISTER, LIST, UPLOAD, DOWNLOAD, REMOVE, RENAME, GIVE_ACCESS, REVOKE_ACCESS};

class Message
{
    string userId;
    Action action;
    vector<string> parameters;

    friend class boost::serialization::access;
    template <class Archive> void serialize(Archive& ar, const unsigned int version)
    {
        ar & userId;
        ar & action;
        ar & parameters;
    }

    public:
        Message(string userId, Action action, vector<string> parameters);
        Message(Message* message);
        Message(vector<string> parameters);
        ~Message();
        string getUserId();
        Action getAction();
        vector<string>& getParameters();
        string toString();

};
