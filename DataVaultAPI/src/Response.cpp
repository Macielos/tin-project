#include "Response.h"

Response::Response(Status status, string description, vector<string> parameters):
    status(status), description(description), parameters(parameters)
{

}

Response::Response(Status status, string description):
    status(status), description(description)
{

}

Response::Response(Status status):
    status(status), description("")
{

}

Response::Response()
{

}

Response::~Response()
{

}

Status Response::getStatus(){
    return status;
}

string Response::getDescription(){
    return description;
}

vector<string>& Response::getParameters(){
    return parameters;
}

void Response::setStatus(Status status){
    this->status=status;
}

void Response::setDescription(string description){
    this->description=description;
}

void Response::setParameters(vector<string> parameters){
    this->parameters=parameters;
}

string Response::toString(){
    stringstream ss;
    ss << "status = "<<status<<endl;
    ss << "description = "<<description<<endl;
    ss << "parameters:";
    for(unsigned int i=0; i<parameters.size(); ++i){
        ss << " "<<parameters[i];
    }
    ss << endl;
    return ss.str();
}
