#include "Server.h"

Server::Server(short messagePort, short dataPort): messagePort(messagePort), dataPort(dataPort), interrupted(false)
{
    cout<<"Server created"<<endl;
}

Server::~Server()
{
}

void Server::init(){
    this->ioService = new boost::asio::io_service();
}

void Server::listen()
{
    try{
        tcp::acceptor acceptor(*ioService, tcp::endpoint(tcp::v4(), messagePort));

        boost::array<char, 128> messageBuffer;
        boost::system::error_code error;

        while(!interrupted)
        {
            cout<<"Server is listening..."<<endl;
            tcp::socket socket(*ioService);
            acceptor.accept(socket);

            socket.read_some(boost::asio::buffer(messageBuffer), error);
            vector<string> temp;
            Message message(temp);
            cout<<"Message buffer: "<<messageBuffer.data()<<endl;
            deserialize(message, messageBuffer.data());

            messages.push_back(&message);
            cout<<"Messages: "<<messages.size()<<endl;

            cout<<"Message: "<<message.getUserId()<<", "<<message.getAction()<<endl;
            vector<string>& parameters = message.getParameters();
            vector<string>::iterator it;
           for(it=parameters.begin(); it!=parameters.end(); ++it ){
                cout<< *it <<'\n';
           }
           cout<<endl;

            string response = "RESPONSE";

            cout<<"Writing "<<response<<endl;
            write(socket, boost::asio::buffer(response), error);

            cout<<"receiving file"<<endl;
            receiveFile("xyz", "TEST_FILE.txt");
        }
    } catch (exception& e){
        cerr << e.what() << endl;
    }
}

void Server::receiveFile(string userId, string filename)
{
    boost::array<char, 1024> buf;
    size_t file_size = 0;
    try {
        boost::asio::ip::tcp::acceptor acceptor(*ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), dataPort));
        boost::system::error_code error;
        boost::asio::ip::tcp::socket socket(*ioService);
        acceptor.accept(socket);
        std::cout << "get client connection." << std::endl;
        boost::asio::streambuf request_buf;
        boost::asio::read_until(socket, request_buf, "\n\n");
        std::cout<< "request size:" << request_buf.size() << "\n";
        std::istream request_stream(&request_buf);
        std::string file_path;
        request_stream >> file_path;
        request_stream >> file_size;
        request_stream.read(buf.c_array(), 2); // eat the "\n\n"

        std::cout << file_path << " size is " << file_size << std::endl;
        //size_t pos = file_path.find_last_of(`\\`);
        //if (pos!=std::string::npos)
        //    file_path = file_path.substr(pos+1);
        std::ofstream output_file(file_path.c_str(), std::ios_base::binary);
        if (!output_file)
        {
            std::cout << "failed to open " << file_path << std::endl;
            return;
        }

        // write extra bytes to file
        do
        {
            request_stream.read(buf.c_array(), (std::streamsize)buf.size());
            std::cout << __FUNCTION__ << " write " << request_stream.gcount() << " bytes.\n";
            output_file.write(buf.c_array(), request_stream.gcount());
        } while (request_stream.gcount()>0);

        for (;;)
        {
            size_t len = socket.read_some(boost::asio::buffer(buf), error);
            if (len>0)
                output_file.write(buf.c_array(), (std::streamsize)len);
            if (output_file.tellp()== (std::fstream::pos_type)(std::streamsize)file_size)
                break; // file was received
            if (error)
            {
                std::cout << error << std::endl;
                break;
            }
        }
        std::cout << "received " << output_file.tellp() << " bytes.\n";
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

template<typename T> void Server::deserialize(T& t, string serializedData)
{
    try{
        std::istringstream archive_stream(serializedData);
        boost::archive::text_iarchive archive(archive_stream);
        archive >> t;
    }catch (std::exception& e){
        boost::system::error_code error(boost::asio::error::invalid_argument);
    }
}
