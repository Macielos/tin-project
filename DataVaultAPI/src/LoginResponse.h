#include <../DataVaultAPI/src/Response.h>
#include <../DataVaultAPI/src/History.h>

class LoginResponse : public Response
{
        History history;

        friend class boost::serialization::access;
        template<class Archive> void serialize(Archive& ar, const unsigned int version) {
            ar & boost::serialization::make_nvp("Response", boost::serialization::base_object<Response>(*this));
            ar & history;
        }

    public:
        LoginResponse(Status status);
        LoginResponse();
        ~LoginResponse();
        void setHistory(History* history);
        History& getHistory();
};
