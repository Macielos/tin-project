#include <../DataVaultAPI/src/Response.h>
#include <../DataVaultAPI/src/History.h>

class LoginResponse : public Response
{
        History history;
        string sessionId;

        friend class boost::serialization::access;
        template<class Archive> void serialize(Archive& ar, const unsigned int version) {
            ar & boost::serialization::make_nvp("Response", boost::serialization::base_object<Response>(*this));
            ar & sessionId;
            ar & history;
        }

    public:
        LoginResponse(Status status, string sessionId);
        LoginResponse();
        ~LoginResponse();
        void setHistory(History* history);
        void setSessionId(string sessionId);
        History& getHistory();
        string getSessionId();
        string toString2();
};
