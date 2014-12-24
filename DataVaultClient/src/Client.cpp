#include "Client.h"

Client::Client()
{

}

Client::~Client()
{
    delete dataSocket;
    delete messageSocket;
    delete ioService;
}

void Client::init(string host, short messagePort, short dataPort){
    this->host = host;
    this->messagePort = messagePort;
    this->dataPort = dataPort;

    cout<<"connecting"<<endl;
    ioService = new boost::asio::io_service();

    tcp::resolver resolver(*ioService);
    tcp::resolver::query messageQuery(host, boost::lexical_cast<std::string>(messagePort));
    tcp::resolver::iterator messageEndpointIterator = resolver.resolve(messageQuery);

    messageSocket = new tcp::socket(*ioService);
    dataSocket = new tcp::socket(*ioService);

    boost::asio::connect(*messageSocket, messageEndpointIterator);
}

string Client::send(Message& message){
    boost::array<char, 128> messageBuffer;
    boost::system::error_code error;

    try{
        cout<<"Writing message of "<<message.getUserId()<<endl;
        string serializedMessage = serialize(message);
        write(*messageSocket, boost::asio::buffer(serializedMessage), error);
        messageSocket->read_some(boost::asio::buffer(messageBuffer), error);
        cout<<"Reading "<<messageBuffer.data()<<endl;
        if (error){
            throw boost::system::system_error(error);
        }
    }catch (std::exception& e){
        std::cerr << e.what() << std::endl;
    }
    return string(messageBuffer.data());
}

void Client::sendFile(string filename){
    try{
        boost::system::error_code error = boost::asio::error::host_not_found;

        tcp::resolver resolver(*ioService);
        tcp::resolver::query dataQuery(host, boost::lexical_cast<std::string>(dataPort));
        tcp::resolver::iterator dataEndpointIterator = resolver.resolve(dataQuery);

        dataSocket = new tcp::socket(*ioService);

        boost::asio::connect(*dataSocket, dataEndpointIterator);

/*        while (error && endpoint_iterator != end)
        {
            socket.close();
            socket.connect(*endpoint_iterator++, error);
        }
        if (error)
            return __LINE__;*/
        boost::array<char, 1024> buf;
        std::ifstream sourceFile(filename.c_str(), std::ios_base::binary | std::ios_base::ate);
        if (!sourceFile){
            std::cout << "Program failed to open " << filename << std::endl;
            return;
        }
        size_t fileSize = sourceFile.tellg();
        sourceFile.seekg(0);
        // first send file name and file size to server
        boost::asio::streambuf request;
        std::ostream request_stream(&request);
        request_stream << filename << "\n"
            << fileSize << "\n\n";
        boost::asio::write(*dataSocket, request);
        std::cout << "start sending file content.\n";
        while(!sourceFile.eof()){
            sourceFile.read(buf.c_array(), (std::streamsize)buf.size());
            if (sourceFile.gcount()<=0)
            {
                std::cout << "read file error " << std::endl;
                return;
            }
            boost::asio::write(*dataSocket, boost::asio::buffer(buf.c_array(),
                sourceFile.gcount()),
                boost::asio::transfer_all(), error);
            if (error)
            {
                std::cout << "send error:" << error << std::endl;
                return;
            }
        }
        std::cout << "Sending file " << filename << " completed successfully.\n";
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}


template<typename T> string Client::serialize(T& t){
    std::ostringstream archive_stream;
    boost::archive::text_oarchive archive(archive_stream);
    archive << t;
    return archive_stream.str();
}
