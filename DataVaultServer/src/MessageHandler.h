#include "ServerStore.h"
#include "../DataVaultAPI/src/Message.h"

#include <queue>

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include <ctime>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class MessageHandler
{
    ServerStore& serverStore;

    //deque<Message*> messageQueue;
    boost::asio::io_service& ioService;

    boost::mutex queueAccessMutex;
    boost::mutex queueEmptyMutex;
    boost::condition_variable_any queueEmpty;

    short dataPort;

    public:
        queue<Message*> messageQueue;

        MessageHandler(ServerStore& serverStore, boost::asio::io_service& ioService, short dataPort); //ServerStore& serverStore, queue& messageQueue);
        ~MessageHandler();

        void run();
        void add(Message* message);
        int getMessageQueueSize();

        void handleMessageThread();
        void handleMessage(Message* message);

    private:
        void receiveFile(string userId, string filename);
};
