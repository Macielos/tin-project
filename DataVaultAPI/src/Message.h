#include <vector>
#include <string>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

using namespace std;

enum Action {REGISTER, LOGIN, LOGOUT, UNREGISTER, LIST, UPLOAD, DOWNLOAD, REMOVE, RENAME, GIVE_ACCESS, REVOKE_ACCESS};

class Message
{
    string userId;
    Action action;
    vector<string>& parameters;

    friend class boost::serialization::access;

    public:
        Message(string userId, Action action, vector<string>& parameters);
        Message(vector<string>& parameters);
        ~Message();
        string getUserId();
        Action getAction();
        vector<string>& getParameters();
        template <typename Archive> void serialize(Archive& ar, const unsigned int version);
};
