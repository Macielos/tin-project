#include "FileTransferManager.h"

FileTransferManager::FileTransferManager(boost::asio::io_service& ioService, short dataPort):
    ioService(ioService), dataPort(dataPort)
{

}

FileTransferManager::~FileTransferManager()
{

}

void FileTransferManager::sendFile(string filename){
    try{
        boost::system::error_code error = boost::asio::error::host_not_found;

        tcp::resolver resolver(ioService);
        tcp::resolver::query dataQuery(host, boost::lexical_cast<string>(dataPort));
        tcp::resolver::iterator dataEndpointIterator = resolver.resolve(dataQuery);

        boost::asio::ip::tcp::socket dataSocket(ioService);

        boost::asio::connect(dataSocket, dataEndpointIterator);

        boost::array<char, 1024> buf;
        std::ifstream sourceFile(filename.c_str(), std::ios_base::binary | std::ios_base::ate);
        if (!sourceFile){
            std::cout << "Program failed to open " << filename << std::endl;
            return;
        }
        size_t fileSize = sourceFile.tellg();
        sourceFile.seekg(0);
        // first send file size to server
        boost::asio::streambuf request;
        std::ostream request_stream(&request);
        request_stream << filename << "\n"
            << fileSize << "\n\n";
        boost::asio::write(dataSocket, request);
        std::cout << "start sending content of "<<filename<<endl;
        while(!sourceFile.eof()){
            sourceFile.read(buf.c_array(), (std::streamsize)buf.size());
            if (sourceFile.gcount()<=0)
            {
                std::cout << "read file error " << std::endl;
                return;
            }
            boost::asio::write(dataSocket, boost::asio::buffer(buf.c_array(),
                sourceFile.gcount()),
                boost::asio::transfer_all(), error);
            if (error)
            {
                std::cout << "send error:" << error << std::endl;
                return;
            }
        }
        std::cout << "Sending file " << filename << " completed.\n";
        dataSocket.close();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void FileTransferManager::receiveFile(string filename)
{
    cout<<"receiving"<<endl;
    boost::array<char, 1024> buf;
    size_t fileSize = 0;
    try {
        boost::system::error_code error;
        boost::asio::ip::tcp::acceptor acceptor(ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), dataPort));
        boost::asio::ip::tcp::socket socket(ioService);
        cout<<"ready"<<endl;
        acceptor.accept(socket);

        std::cout << "get client connection." << std::endl;
        boost::asio::streambuf request_buf;
        boost::asio::read_until(socket, request_buf, "\n\n");
        std::cout<< "request size:" << request_buf.size() << "\n";
        std::istream request_stream(&request_buf);
        request_stream >> fileSize;
        request_stream.read(buf.c_array(), 2); // eat the "\n\n"

        std::cout << filename << " size is " << fileSize << std::endl;
        //size_t pos = file_path.find_last_of(`\\`);
        //if (pos!=std::string::npos)
        //    file_path = file_path.substr(pos+1);
        std::ofstream output_file(filename.c_str(), std::ios_base::binary);
        if (!output_file)
        {
            std::cout << "failed to open " << filename << std::endl;
            return;
        }

        // write extra bytes to file
        do
        {
            request_stream.read(buf.c_array(), (std::streamsize)buf.size());
            std::cout << __FUNCTION__ << " write " << request_stream.gcount() << " bytes.\n";
            output_file.write(buf.c_array(), request_stream.gcount());
        } while (request_stream.gcount()>0);

        while(true)
        {
            size_t len = socket.read_some(boost::asio::buffer(buf), error);
            if (len>0)
                output_file.write(buf.c_array(), (std::streamsize)len);
            if (output_file.tellp()== (std::fstream::pos_type)(std::streamsize)fileSize){
                socket.close();
                break; // file was received
            }
            if (error)
            {
                std::cout << error << std::endl;
                break;
            }
        }
        std::cout << "received " << output_file.tellp() << " bytes."<<endl;

    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void FileTransferManager::confirm(){
    cout<<"connecting..."<<endl;
    tcp::resolver resolver(ioService);
    tcp::resolver::query query(host, boost::lexical_cast<std::string>(dataPort+1));
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    tcp::socket socket(ioService);
    boost::asio::connect(socket, endpoint_iterator);
    cout<<"connected"<<endl;
}
