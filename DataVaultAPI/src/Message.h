#include <vector>
#include <string>
#include <sstream>

#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

#include "Constants.h"

#pragma once

using namespace std;
using namespace boost::archive;

class Message
{
    string userId;
    string source;
    Action action;
    vector<string> parameters;

    friend class boost::serialization::access;
    template <class Archive> void serialize(Archive& ar, const unsigned int version)
    {
        ar & userId;
        ar & source;
        ar & action;
        ar & parameters;
    }

    public:
        Message(string userId, string source, Action action, vector<string> parameters);
        Message(Message& message);
        Message(vector<string> parameters);
        ~Message();
        string getUserId();
        string getSource();
        Action getAction();
        vector<string>& getParameters();
        string toString();

};
