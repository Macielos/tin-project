#include "LoginResponse.h"

LoginResponse::LoginResponse(Status status):
    Response(status)
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

History& LoginResponse::getHistory(){
    return history;
}
