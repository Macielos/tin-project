#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

using namespace std;

class Serializer
{
    public:
        Serializer();
        ~Serializer();
        template<typename T> string serialize(T& t);
        template<typename T> void deserialize(T& t, string serializedData);
};
