#include "Message.h"

Message::Message(string userId, string source, Action action, vector<string> parameters):
    userId(userId), source(source), action(action), parameters(parameters)
{
}

Message::Message(Action action, vector<string> parameters):
    action(action), parameters(parameters)
{
}

Message::Message()
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

string Message::getSessionId(){
    return sessionId;
}

Action Message::getAction(){
    return action;
}

vector<string>& Message::getParameters(){
    return parameters;
}

void Message::setUserId(string userId){
    this->userId=userId;
}

void Message::setSource(string source){
    this->source=source;
}

void Message::setSessionId(string sessionId){
    this->sessionId=sessionId;
}

void Message::setAction(Action action){
    this->action=action;
}

void Message::setParameters(vector<string> parameters){
    this->parameters=parameters;
}

string Message::toString(){
    stringstream ss;
    ss << "userId = "<<userId<<endl;
    ss << "source = "<<source<<endl;
    ss << "sessionId = "<<sessionId<<endl;
    ss << "action = "<<action<<endl;
    ss << "parameters:";
    for(unsigned int i=0; i<parameters.size(); ++i){
        ss << " "<<parameters[i];
    }
    ss << endl;
    return ss.str();
}
