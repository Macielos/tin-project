/**
 *  ### Client.cpp ###
 *
 *      Ciało klasy Client.
 *
 */
#include "Client.h"

Client::Client()
{
    host = "";
    messagePort = 0;
    dataPort = 0;
    userId = "";
    password = "";

    validParameters = false;
    connected = false;
    logged = false;

    ioService = new boost::asio::io_service();
}

Client::~Client()
{
 //   delete socket;
    delete ioService;
}

/*
    SETery parametrów połączenia. Wartość logiczna określa czy parametr został dodany poprawnie.
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
        checkParamCorrectness();
        return true;
    }
    else
    {
        return false;
    }
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

/**
 *  Sprawdza czy wszystkie parametry połączenia są poprawne. W zależności ustawia flagę.
 */
void Client::checkParamCorrectness()
{
    if ( (host.length() > 3)
        && ((dataPort > 1023) && (dataPort < 65536))
        && ((messagePort > 1023) && (messagePort < 65536)) )
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
        tcp::resolver resolver(*ioService);
        tcp::resolver::query query(host, boost::lexical_cast<std::string>(messagePort));
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

        socket = new tcp::socket(*ioService);

        boost::asio::connect(*socket, endpoint_iterator);

        connected = true;
        return true;
    }
    catch (boost::system::system_error err)
    {
        cout << "# BŁĄD: Nie można nazwiązać połączenia z serwerem!\n";
    }
    return false;
}

string Client::sendMessage(Message& message)
{
    boost::array<char, 128> messageBuffer;
    boost::system::error_code error;

    try
    {
        cout << "Wysyłanie polecenia od: " << message.getUserId() << endl;
        string serializedMessage = serialize(message);
        write(*socket, boost::asio::buffer(serializedMessage), error);
        socket->read_some(boost::asio::buffer(messageBuffer), error);

        if (error)
        {
            throw boost::system::system_error(error);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return string(messageBuffer.data());
}

template<typename T> string Client::serialize(T& t)
{
    std::ostringstream archive_stream;
    boost::archive::text_oarchive archive(archive_stream);
    archive << t;
    return archive_stream.str();
}
