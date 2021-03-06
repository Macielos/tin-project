/**
 *  ### Client.cpp ###
 *
 *      Ciało klasy Client.
 *
 */
#include "Client.h"

Client::Client(): fileTransferManager(ioService), socket(ioService)
{
    host = "";
    messagePort = 0;
    dataPort = 0;
    sessionId = "";
    userId = "";
    password = "";

    validParameters = false;
    connected = false;
    logged = false;
}

Client::~Client()
{
}

/**
 *   SETery parametrów połączenia. Wartość logiczna określa czy parametr został dodany poprawnie.
 */
bool Client::setHost(string host)
{
    if ( (host.length() > 3) && (host.length() < 100) )
    {
        this->host = host;
        checkParamCorrectness();
        return true;
    }
    else
    {
        return false;
    }
}

bool Client::setMessagePort(int messagePort)
{
    if ( (messagePort > 1023) && (messagePort < 65536) )
    {
        this->messagePort = messagePort;
        checkParamCorrectness();
        return true;
    }
    else
    {
        return false;
    }
}

bool Client::setDataPort(int dataPort)
{
    if ( (dataPort > 1023) && (dataPort < 65536) )
    {
        this->dataPort = dataPort;
        fileTransferManager.setDataPort(dataPort);
        checkParamCorrectness();
        return true;
    }
    else
    {
        return false;
    }
}

bool Client::setNotificationPort(int notificationPort)
{
    if ( (dataPort > 1023) && (dataPort < 65536) )
    {
        this->notificationPort = notificationPort;
        fileTransferManager.setNotificationPort(notificationPort);
        checkParamCorrectness();
        return true;
    }
    else
    {
        return false;
    }
}

void Client::setUserId(string userId){
    this->userId = userId;
}

/*
    GETery flag.
*/
bool Client::isValidParameters()
{
    return validParameters;
}

bool Client::isConnected()
{
    return connected;
}

bool Client::isLogged()
{
    return logged;
}

void Client::setLogged(bool logged){
    this->logged = logged;
}

void Client::setSessionId(string sessionId){
    this->sessionId = sessionId;
}

/**
 *  Sprawdza czy wszystkie parametry połączenia są poprawne. W zależności ustawia flagę.
 */
void Client::checkParamCorrectness()
{
    if ( host.length() > 3
        && dataPort > 1023 && dataPort < 65536
        && messagePort > 1023 && messagePort < 65536
        && notificationPort > 1023 && notificationPort < 65536
        )
    {
        validParameters = true;
    }
    else
    {
        validParameters = false;
    }
}

/**
 *  Nawiązuje połączenie z serwerem.
 */
bool Client::connect()
{
    connected = false;
    if (validParameters == false) // jeśli (parametry nie poprawne) nic nie rób
    {
        return false;
    }
    try
    {
        tcp::resolver resolver(ioService);
        tcp::resolver::query query(host, boost::lexical_cast<std::string>(messagePort));
        boost::asio::ip::tcp::resolver::iterator destination = resolver.resolve(query);
        boost::asio::ip::tcp::resolver::iterator end;

        while ( destination != end )
        {
            endpoint = *destination++;
        }

        tcp::socket socket(ioService);
        socket.connect(endpoint);

        connected = true;
        return true;
    }
    catch (boost::system::system_error error)
    {
        cerr<<"#BŁĄD: Wystąpił nieoczekiwany wyjątek podczas nawiązywania połączenia z serwerem."<<endl;
        return false;
    }

}

Response* Client::sendMessage(Message& message)
{
    boost::array<char, 8192> responseBuffer;
    boost::system::error_code error;

    // otwarcie gniazda
    socket.connect(endpoint);

    message.setUserId(userId);
    message.setSource(socket.remote_endpoint().address().to_string());
    message.setSessionId(sessionId);

    string serializedMessage = serialize(message);
    write(socket, boost::asio::buffer(serializedMessage), error);
    socket.read_some(boost::asio::buffer(responseBuffer), error);
    if (error)
    {
        cerr<<error<<endl;
        return NULL;
    }

    if(message.getAction()==LOGIN){
        LoginResponse* loginResponse = new LoginResponse();
        deserialize(*loginResponse, responseBuffer.data());
        sessionId = loginResponse->getSessionId();
        socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
        socket.close();
        return loginResponse;
    }else{
        Response* response = new Response();
        deserialize(*response, responseBuffer.data());
        socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
        socket.close();
        return response;
    }
}

bool Client::sendFile(string filename, bool notify)
{
    return fileTransferManager.sendFile(host, filename, notify);
}

bool Client::receiveFile(string filename, bool notify)
{
    return fileTransferManager.receiveFile(host, filename, notify);
}

template<typename T> string Client::serialize(T& t)
{
    std::ostringstream archive_stream;
    boost::archive::text_oarchive archive(archive_stream);
    archive << t;
    return archive_stream.str();
}

template<typename T> void Client::deserialize(T& t, string serializedData)
{
    std::istringstream archive_stream(serializedData);
    boost::archive::text_iarchive archive(archive_stream);
    archive >> t;
}
