#include "Message.h"

Message::Message(string userId, Action action, vector<string>& parameters): userId(userId), action(action), parameters(parameters)
{
    //ctor
}

Message::Message(vector<string>& parameters): parameters(parameters)
{
    //ctor
}

Message::~Message()
{
    //dtor
}

string Message::getUserId(){
    return userId;
}

Action Message::getAction(){
    return action;
}

vector<string>& Message::getParameters(){
    return parameters;
}

template <typename Archive> void Message::serialize(Archive& ar, const unsigned int version)
{
    ar & userId;
    ar & action;
    ar & parameters;
}
