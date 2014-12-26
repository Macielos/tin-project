#include "Message.h"

Message::Message(string userId, Action action, vector<string> parameters): userId(userId), action(action), parameters(parameters)
{
    //ctor
}

Message::Message(Message* message): userId(message->getUserId()), action(message->getAction()), parameters(message->getParameters())
{

}

Message::Message(vector<string> parameters): parameters(parameters)
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

string Message::toString(){
    stringstream ss;
    ss << "userId="<<userId<<endl;
    ss << "action="<<action<<endl;
    ss << "parameters:"<<endl;
    for(unsigned int i=0; i<parameters.size(); ++i){
        ss << "  "<<parameters[i]<<endl;
    }
    return ss.str();
}
