/**
 *  ### Client.cpp ###
 *
 *      Ciało klasy Client.
 *
 */
 #include "Client.h"

Client::Client()
{

}

Client::~Client()
{
    delete socket;
    delete ioService;
}

void Client::setHost(string host)
{
    this->host = host;
}

void Client::setMessagePort(short messagePort)
{
    this->messagePort = messagePort;
}

void Client::setDataPort(short dataPort)
{
    this->dataPort = dataPort;
}

void Client::init()
{
    cout << "Nawiązywanie połączenia..." << endl;
    ioService = new boost::asio::io_service();

    tcp::resolver resolver(*ioService);
    tcp::resolver::query query(host, boost::lexical_cast<std::string>(messagePort));
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    socket = new tcp::socket(*ioService);

    boost::asio::connect(*socket, endpoint_iterator);
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
        cout << "Serwer: " << messageBuffer.data() << endl;
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
