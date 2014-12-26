#include "MessageHandler.h"

MessageHandler::MessageHandler(ServerStore& serverStore, boost::asio::io_service& ioService, short dataPort):
    serverStore(serverStore), ioService(ioService), dataPort(dataPort)
{
}

MessageHandler::~MessageHandler()
{
}

/*void MessageHandler::run(){
    cout<<"Starting thread"<<endl;
    boost::thread messageHandleThread(boost::bind(&MessageHandler::handleMessageThread, this));
}

void MessageHandler::add(Message* message){
    queueAccessMutex.lock();
    messageQueue.push(message);
    queueEmpty.notify_all();
    queueAccessMutex.unlock();
}

void MessageHandler::handleMessageThread(){
    while(true){
        queueAccessMutex.lock();
        while(messageQueue.size()==0){
            cout<<"Thread is waiting"<<endl;
            cout<<messageQueue.size()<<endl;
            queueAccessMutex.unlock();
            queueEmpty.wait(queueEmptyMutex);
            queueAccessMutex.lock();
            cout<<messageQueue.size()<<endl;
            cout<<"Thread is awakening"<<endl;
        }
        cout<<messageQueue.size()<<endl;

        Message* message = messageQueue.front();
        messageQueue.pop();

        //TU DOSTAJE SEGFAULTA
        cout<<"message: "<<message->toString()<<endl;
        queueAccessMutex.unlock();




        handleMessage(message);
    }
}*/

void MessageHandler::run(){
    cout<<"Starting thread"<<endl;
    boost::thread messageHandleThread(boost::bind(&MessageHandler::handleMessageThread, this));
}

void MessageHandler::add(Message* message){
    queueAccessMutex.lock();
    messageQueue.push(message);
    queueEmpty.notify_all();
    cout<<"elements: "<<messageQueue.size()<<endl;
    queueAccessMutex.unlock();
}

void MessageHandler::handleMessageThread(){
    while(true){
        queueAccessMutex.lock();
        while(messageQueue.size()==0){
            cout<<"Thread is waiting"<<endl;
            queueAccessMutex.unlock();
            queueEmpty.wait(queueEmptyMutex);
            queueAccessMutex.lock();
            cout<<"Thread is awakening"<<endl;
        }
        Message* message = messageQueue.front();

        //cout<<message->getUserId()<<endl;
        cout<<message->getAction()<<endl;
        cout<<message->getParameters().size()<<endl;
        cout<<message->getParameters()[0]<<endl;

        messageQueue.pop();
        queueAccessMutex.unlock();
        boost::this_thread::sleep_for(boost::chrono::seconds(5));
        cout<<"processed"<<endl;
    }
}

void MessageHandler::handleMessage(Message* message){
    vector<string>& parameters = message->getParameters();
    vector<string>::iterator it;

    cout<<"Handling message"<<endl;

    switch(message->getAction()){
    case UPLOAD:
        cout<<"Uploading file"<<endl;
        cout<<message->toString()<<endl;
        for(it=parameters.begin(); it!=parameters.end(); ++it ){
            receiveFile(message->getUserId(), *it);
        }
        break;
    default:
        break;
    }
}

void MessageHandler::receiveFile(string userId, string filename)
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

        while(true)
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

int MessageHandler::getMessageQueueSize(){
    return messageQueue.size();
}
