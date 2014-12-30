#include "Message.h"

Message::Message(string userId, string source, Action action, vector<string> parameters): userId(userId), source(source), action(action), parameters(parameters)
{
}

Message::Message(Message& message): userId(message.getUserId()), source(message.getSource()), action(message.getAction()), parameters(message.getParameters())
{
}

Message::Message(vector<string> parameters): parameters(parameters)
{
}

Message::~Message()
{
}

string Message::getUserId(){
    return userId;
}

string Message::getSource(){
    return source;
}

Action Message::getAction(){
    return action;
}

vector<string>& Message::getParameters(){
    return parameters;
}

string Message::toString(){
    stringstream ss;
    ss << "userId = "<<userId<<endl;
    ss << "source = "<<source<<endl;
    ss << "action = "<<action<<endl;
    ss << "parameters:";
    for(unsigned int i=0; i<parameters.size(); ++i){
        ss << " "<<parameters[i];
    }
    ss << endl;
    return ss.str();
}
