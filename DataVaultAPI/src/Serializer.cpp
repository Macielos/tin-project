#include "Serializer.h"

Serializer::Serializer()
{
    //ctor
}

Serializer::~Serializer()
{
    //dtor
}

template<typename T> string Serializer::serialize(T& t)
{
    std::ostringstream archive_stream;
    boost::archive::text_oarchive archive(archive_stream);
    archive << t;
    return archive_stream.str();
}

template<typename T> void Serializer::deserialize(T& t, string serializedData)
{
    std::istringstream archive_stream(serializedData);
    boost::archive::text_iarchive archive(archive_stream);
    archive >> t;
}
