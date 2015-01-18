#include "LoginResponse.h"

LoginResponse::LoginResponse(Status status, string sessionId):
    Response(status), sessionId(sessionId)
{

}

LoginResponse::LoginResponse():
    Response()
{

}

LoginResponse::~LoginResponse()
{

}

void LoginResponse::setHistory(History* history)
{
    this->history = *history;
}

void LoginResponse::setSessionId(string sessionId)
{
    this->sessionId = sessionId;
}

History& LoginResponse::getHistory(){
    return history;
}

string LoginResponse::getSessionId(){
    return sessionId;
}


string LoginResponse::toString2(){
    stringstream ss;
    ss << "status = "<<status<<endl;
    ss << "description = "<<description<<endl;
    ss << "sessionId = "<<sessionId<<endl;
    ss << "historySize = "<<history.size()<<endl;
    ss << "parameters:";
    for(unsigned int i=0; i<parameters.size(); ++i){
        ss << " "<<parameters[i];
    }
    ss << endl;
    return ss.str();
}
