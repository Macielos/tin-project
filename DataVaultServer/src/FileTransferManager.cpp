#include "FileTransferManager.h"

FileTransferManager::FileTransferManager(boost::asio::io_service& ioService, short dataPort):
    ioService(ioService), dataPort(dataPort)
{

}

FileTransferManager::~FileTransferManager()
{

}

void FileTransferManager::sendFile(string destination, string filename){
    try{

        boost::system::error_code error = boost::asio::error::host_not_found;
        //boost::array<char, 128> messageBuffer;

        tcp::resolver resolver(ioService);
        tcp::resolver::query dataQuery(destination, boost::lexical_cast<string>(dataPort+1));
        tcp::resolver::iterator dataEndpointIterator = resolver.resolve(dataQuery);

        boost::asio::ip::tcp::socket dataSocket(ioService);

        boost::asio::connect(dataSocket, dataEndpointIterator);

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
        boost::asio::write(dataSocket, request);
        std::cout << "start sending file content.\n";
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
        std::cout << "Sending file " << filename << " completed successfully.\n";
        dataSocket.close();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void FileTransferManager::receiveFile(string source, string filename)
{
    boost::array<char, 1024> buf;
    size_t file_size = 0;
    try {
        boost::asio::ip::tcp::acceptor acceptor(ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), dataPort));
        boost::system::error_code error;
        boost::asio::ip::tcp::socket socket(ioService);
        acceptor.accept(socket);
        std::cout << "get client connection." << std::endl;
        boost::asio::streambuf request_buf;
        boost::asio::read_until(socket, request_buf, "\n\n");
        std::cout<< "request size:" << request_buf.size() << "\n";
        std::istream request_stream(&request_buf);
        request_stream >> file_size;
        request_stream.read(buf.c_array(), 2); // eat the "\n\n"

        std::cout << filename << " size is " << file_size << std::endl;
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
            if (output_file.tellp()== (std::fstream::pos_type)(std::streamsize)file_size){
                size_t len2 = socket.write_some(boost::asio::buffer("OK"), error);
                cout<<"response sent: "<<len2<<endl;
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
