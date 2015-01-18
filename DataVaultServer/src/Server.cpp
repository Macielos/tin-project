#include "Server.h"

Server::Server(int messagePort, int dataPort, int notificationPort):
    fileTransferManager(ioService, dataPort, notificationPort),
    messagePort(messagePort), dataPort(dataPort), notificationPort(notificationPort),
    interrupted(false), SESSION_ID_LENGTH(32)
{
    cout<<"Serwer uruchomiony"<<endl;
}

Server::~Server()
{
}

/**
 *  Główna pętla serwera. Serwer bedzie nasłuchiwal przychodzących połączeń. Dla nowego połączenia
 *  zostanie uruchomiony nowy wątek, który je obsłuży przy użyciu osobnego gniazda.
 */
void Server::listen()
{
    try{
        tcp::acceptor acceptor(ioService, tcp::endpoint(tcp::v4(), messagePort));
        while(!interrupted)
        {
            cout<<"Serwer nasłuchuje..."<<endl;
            tcp::socket* socket = new tcp::socket(ioService);
            acceptor.accept(*socket);
            boost::thread messageHandler(boost::bind(&Server::handleMessage, this, socket));
        }
    } catch (exception& e){
        cerr << e.what() << endl;
    }
}

/**
 *  Wątek obsługujący pojedynczą wiadomość przesłaną do serwera.
 */
void Server::handleMessage(tcp::socket* socket){
    boost::array<char, 2048> messageBuffer;
    boost::system::error_code error;
    ioService.run();
    socket->read_some(boost::asio::buffer(messageBuffer), error);
    if (error){
        cerr<<error<<endl;
        socket->close();
        delete socket;
        return;
    }

    Message message;
    deserialize(message, messageBuffer.data());

    cout<<"Otrzymano wiadomość: "<<endl;
    cout<<message.toString()<<endl;

    int result;
    string response;
    string sessionId;
    History emptyHistory;
    vector<string> mismatchingParameters;
    History* history;

    if(message.getAction()!=REGISTER
       && message.getAction()!=LOGIN
       && message.getAction()!=UNREGISTER
       && (serverStore.getSessionId(message.getUserId())!=message.getSessionId()
        || serverStore.getSessionId(message.getUserId())==""))
    {
        response = createResponse(NOT_LOGGED_IN);
        socket->write_some(boost::asio::buffer(response), error);
        socket->close();
        delete socket;
        return;
    }

    cout<<"session id = "<<serverStore.getSessionId(message.getUserId())<<endl;

    switch(message.getAction()){
        case REGISTER:
        {
            // muszą być dwa parametry
            if (message.getParameters().size() != 2)
            {
                response = createResponse(WRONG_SYNTAX);
                socket->write_some(boost::asio::buffer(response), error);
            }
            else
            {
                // tworzymy użyszkodnika
                result = serverStore.registerUser(message.getParameters()[0], message.getParameters()[1]);
                switch (result)
                {
                    case 0: // użytkownik dodany poprawnie
                        response = createResponse(OK);
                        break;
                    case -1: // login zajęty
                        response = createResponse(INCORRECT_LOGIN);
                        break;
                }
                socket->write_some(boost::asio::buffer(response), error);
            }
            break;
        }
        case LOGIN:
        {
            // muszą być dwa parametry
            if (message.getParameters().size() != 2)
            {
                response = createResponse(WRONG_SYNTAX, "", &emptyHistory);
                socket->write_some(boost::asio::buffer(response), error);
            }
            else
            {
                // logowanie użyszkodnika
                result = serverStore.loginUser(message.getParameters()[0], message.getParameters()[1]);
                switch (result)
                {
                    case 0: // użytkownik zalogowany poprawnie
                        history = serverStore.getHistory(message.getUserId());
                        if(history==NULL){
                            history = &emptyHistory;
                        }
                        sessionId = generateSessionId();
                        serverStore.setSessionId(message.getUserId(), sessionId);
                        response = createResponse(OK, sessionId, history);
                        serverStore.clearHistory(message.getUserId());
                        break;
                    case -1: // niepoprawny login
                        response = createResponse(INCORRECT_LOGIN, "", &emptyHistory);
                        break;
                    case -2: // niepoprawne haslo
                        response = createResponse(INCORRECT_PASSWORD, "", &emptyHistory);
                        break;
                }
                socket->write_some(boost::asio::buffer(response), error);
            }
            break;
        }
        case LOGOUT:
        {
            serverStore.setSessionId(message.getUserId(), "");
            response = createResponse(OK);
            socket->write_some(boost::asio::buffer(response), error);
            break;
        }
        case UNREGISTER:
        {
            // muszą być dwa parametry
            if (message.getParameters().size() != 2)
            {
                response = createResponse(WRONG_SYNTAX);
                socket->write_some(boost::asio::buffer(response), error);
            }
            else
            {
                // usuwamy użyszkodnika
                result = serverStore.unregisterUser(message.getParameters()[0], message.getParameters()[1]);
                switch (result)
                {
                    case 0: // użytkownik usunięty poprawnie
                        response = createResponse(OK);
                        break;
                    case -1: // login błędny
                        response = createResponse(INCORRECT_LOGIN);
                        break;
                    case -2: // hasło błędne
                        response = createResponse(INCORRECT_PASSWORD);
                        break;
                }
                socket->write_some(boost::asio::buffer(response), error);
            }
            break;
        }
        case LIST:
        {
            //jeśli podano parametry - błąd
            if(message.getParameters().size()!=0){
                response = createResponse(WRONG_SYNTAX);
                socket->write_some(boost::asio::buffer(response), error);
                break;
            }

            vector<string> filenames = serverStore.list(message.getUserId());
            response = createResponse(OK, filenames);
            socket->write_some(boost::asio::buffer(response), error);
            filenames.clear();
            break;
        }
        case LIST_SHARED:
        {
            //jeśli podano parametry - błąd
            if(message.getParameters().size()!=0){
                response = createResponse(WRONG_SYNTAX);
                socket->write_some(boost::asio::buffer(response), error);
                break;
            }

            vector<string> names = serverStore.listShared(message.getUserId());
            response = createResponse(OK, names);
            socket->write_some(boost::asio::buffer(response), error);
            names.clear();
            break;
        }
        case UPLOAD:
            //jeśli nie podano parametrów - błąd
            if(message.getParameters().size()==0){
                response = createResponse(WRONG_SYNTAX);
                socket->write_some(boost::asio::buffer(response), error);
                break;
            }



            //żądanie poprawne, rozpocznij transfer
            dataPortAccessMutex.lock();
            response = createResponse(OK);
            socket->write_some(boost::asio::buffer(response), error);
            for(unsigned int i=0; i<message.getParameters().size(); ++i){
                cout<<"Użytkownik "<<message.getUserId()<<" przesyła plik "<<message.getParameters()[i]<<endl;
                result = fileTransferManager.receiveFile(message.getSource(), message.getParameters()[i], i!=0);
                if(!result){
                    cerr<<"Nie przesłano pliku"<<endl;
                    break;
                }
                serverStore.add(message.getUserId(), message.getParameters()[i]);
                if(serverStore.fileExists(message.getUserId(), message.getParameters()[i])){
                    serverStore.updateHistory(FILE_MODIFIED, message.getUserId(), message.getParameters()[i]);
                }
                cout<<"Gotowe."<<endl;
            }
            dataPortAccessMutex.unlock();
            break;

        case DOWNLOAD:
            //jeśli nie podano parametrów - błąd
            if(message.getParameters().size()==0){
                response = createResponse(WRONG_SYNTAX);
                socket->write_some(boost::asio::buffer(response), error);
                break;
            }
            //jeśli któreś z podanych plików nie istnieją - błąd.
            //zwróć nazwy nieistniejących plików.
            for(unsigned int i=0; i<message.getParameters().size(); ++i){
                if(!serverStore.fileExists(message.getUserId(), message.getParameters()[i])){
                    mismatchingParameters.push_back(message.getParameters()[i]);
                }
            }
            if(mismatchingParameters.size()>0){
                response = createResponse(NO_SUCH_FILE, mismatchingParameters);
                socket->write_some(boost::asio::buffer(response), error);
                break;
            }

            //żądanie poprawne, rozpocznij transfer
            dataPortAccessMutex.lock();
            response = createResponse(OK);
            socket->write_some(boost::asio::buffer(response), error);
            for(unsigned int i=0; i<message.getParameters().size(); ++i){
                cout<<"Użytkownik "<<message.getUserId()<<" pobiera plik "<<message.getParameters()[i]<<endl;
                result = fileTransferManager.sendFile(message.getSource(), message.getParameters()[i], true);
                if(!result){
                    cerr<<"Nie przesłano pliku"<<endl;
                    break;
                }
                cout<<"Gotowe."<<endl;
            }
            dataPortAccessMutex.unlock();
            break;

        case DOWNLOAD_SHARED:
            //musi być podany użytkownik oraz min. 1 plik, jeśli nie jest - błąd
            if(message.getParameters().size()<2){
                response = createResponse(WRONG_SYNTAX);
                socket->write_some(boost::asio::buffer(response), error);
                break;
            }

            //jeśli nie ma takiego użytkownika - błąd
            if(!serverStore.userExists(message.getParameters()[0])){
                response = createResponse(NO_SUCH_USER);
                socket->write_some(boost::asio::buffer(response), error);
                break;
            }

            //jeśli któreś z podanych plików nie istnieją - błąd.
            //zwróć nazwy nieistniejących plików.
            for(unsigned int i=1; i<message.getParameters().size(); ++i){
                if(!serverStore.fileExists(message.getParameters()[0], message.getParameters()[i])){
                    mismatchingParameters.push_back(message.getParameters()[i]);
                }
            }
            if(mismatchingParameters.size()>0){
                response = createResponse(NO_SUCH_FILE, mismatchingParameters);
                socket->write_some(boost::asio::buffer(response), error);
                break;
            }

            //jeśli użytkownik nie ma dostępu do któregoś z plików - błąd.
            //zwróć nazwy tychże plików.
            for(unsigned int i=1; i<message.getParameters().size(); ++i){
                if(!serverStore.hasAccess(message.getUserId(), message.getParameters()[0], message.getParameters()[i])){
                    mismatchingParameters.push_back(message.getParameters()[i]);
                }
            }
            if(mismatchingParameters.size()>0){
                response = createResponse(ACCESS_DENIED, mismatchingParameters);
                socket->write_some(boost::asio::buffer(response), error);
                break;
            }

            //żądanie poprawne, rozpocznij transfer
            dataPortAccessMutex.lock();
            response = createResponse(OK);
            socket->write_some(boost::asio::buffer(response), error);
            for(unsigned int i=1; i<message.getParameters().size(); ++i){
                cout<<"Użytkownik "<<message.getUserId()<<" pobiera plik "<<message.getParameters()[i]<<" należący do użytkownika "<<message.getParameters()[0]<<endl;
                result = fileTransferManager.sendFile(message.getSource(), message.getParameters()[i], true);
                if(!result){
                    cerr<<"Nie przesłano pliku"<<endl;
                    break;
                }
                cout<<"Gotowe."<<endl;
            }
            dataPortAccessMutex.unlock();
            break;

        case REMOVE:
            //jeśli nie podano parametrów - błąd
            if(message.getParameters().size()==0){
                response = createResponse(WRONG_SYNTAX);
                socket->write_some(boost::asio::buffer(response), error);
                break;
            }

            //usuń pliki, jeśli istnieją, zapisz nazwy tych, które nie istnieją
            int result;
            for(unsigned int i=0; i<message.getParameters().size(); ++i){
                serverStore.updateHistory(FILE_REMOVED, message.getUserId(), message.getParameters()[i]);
                result = serverStore.remove(message.getUserId(), message.getParameters()[i]);
                if(result==0){
                    remove(message.getParameters()[i].c_str());
                }else{
                    mismatchingParameters.push_back(message.getParameters()[i]);
                }
            }

            //wyślij zwrotny komunikat
            if(mismatchingParameters.size()>0){
                response = createResponse(NO_SUCH_FILE, mismatchingParameters);
            }else{
                response = createResponse(OK);
            }
            socket->write_some(boost::asio::buffer(response), error);
            break;

        case RENAME:
            //jeśli liczba parametrów różna od 2 - błąd
            if(message.getParameters().size()!=2){
                response = createResponse(WRONG_SYNTAX);
                socket->write_some(boost::asio::buffer(response), error);
                break;
            }
            result = serverStore.rename(message.getUserId(),
                message.getParameters()[0], message.getParameters()[1]);
            switch(result){
            case 0:
                serverStore.updateHistory(FILE_RENAMED, message.getUserId(), message.getParameters()[1], message.getParameters()[0]);
                rename(message.getParameters()[0].c_str(), message.getParameters()[1].c_str());
                response = createResponse(OK);
                break;
            case -2:
                mismatchingParameters.push_back(message.getParameters()[0]);
                response = createResponse(NO_SUCH_FILE, mismatchingParameters);
                break;
            case -3:
                mismatchingParameters.push_back(message.getParameters()[1]);
                response = createResponse(FILE_EXISTS, mismatchingParameters);
                break;
            }
            socket->write_some(boost::asio::buffer(response), error);
            break;
        case GIVE_ACCESS:
            if(message.getParameters().size()<2){
                response = createResponse(WRONG_SYNTAX);
                socket->write_some(boost::asio::buffer(response), error);
                break;
            }

            for(unsigned int i=1; i<message.getParameters().size(); ++i){
                result = serverStore.giveAccess(message.getUserId().c_str(), message.getParameters()[0].c_str(), message.getParameters()[i].c_str());
                switch(result){
                case 0:
                    serverStore.updateHistory(ACCESS_GRANTED, message.getUserId(), message.getParameters()[i], message.getParameters()[0]);
                    response = createResponse(OK);
                    break;
                case -1:
                    mismatchingParameters.push_back(message.getParameters()[i]);
                    response = createResponse(NO_SUCH_USER, mismatchingParameters);
                    break;
                case -2:
                    mismatchingParameters.push_back(message.getParameters()[i]);
                    response = createResponse(NO_SUCH_FILE, mismatchingParameters);
                    break;
                case -3:
                    mismatchingParameters.push_back(message.getParameters()[i]);
                    response = createResponse(NO_SUCH_USER, mismatchingParameters);
                    break;
                case -4:
                    mismatchingParameters.push_back(message.getParameters()[i]);
                    response = createResponse(ALREADY_HAVE_ACCESS, mismatchingParameters);
                    break;
                case -5:
                    mismatchingParameters.push_back(message.getParameters()[i]);
                    response = createResponse(OWN_FILE, mismatchingParameters);
                    break;
                }
                if(result!=0){
                    break;
                }
            }
            socket->write_some(boost::asio::buffer(response), error);
            break;
        case REVOKE_ACCESS:
            if(message.getParameters().size()<2){
                response = createResponse(WRONG_SYNTAX);
                socket->write_some(boost::asio::buffer(response), error);
                break;
            }
            for(unsigned int i=1; i<message.getParameters().size(); ++i){
                result = serverStore.revokeAccess(message.getUserId().c_str(), message.getParameters()[0].c_str(), message.getParameters()[i].c_str());
                switch(result){
                case 0:
                    serverStore.updateHistory(ACCESS_REVOKED, message.getUserId(), message.getParameters()[i], message.getParameters()[0]);
                    response = createResponse(OK);
                    break;
                case -1:
                    mismatchingParameters.push_back(message.getParameters()[i]);
                    response = createResponse(NO_SUCH_USER, mismatchingParameters);
                    break;
                case -2:
                    mismatchingParameters.push_back(message.getParameters()[i]);
                    response = createResponse(NO_SUCH_FILE, mismatchingParameters);
                    break;
                case -3:
                    mismatchingParameters.push_back(message.getParameters()[i]);
                    response = createResponse(NO_SUCH_USER, mismatchingParameters);
                    break;
                case -4:
                    mismatchingParameters.push_back(message.getParameters()[i]);
                    response = createResponse(ALREADY_NO_ACCESS, mismatchingParameters);
                    break;
                case -5:
                    mismatchingParameters.push_back(message.getParameters()[i]);
                    response = createResponse(OWN_FILE, mismatchingParameters);
                    break;
                }
                if(result!=0){
                    break;
                }
            }
            socket->write_some(boost::asio::buffer(response), error);
            break;
        default:
            response = createResponse(WRONG_SYNTAX);
            socket->write_some(boost::asio::buffer(response), error);
    }
    socket->close();
    delete socket;
}

string Server::createResponse(Status status){
    Response response(status);
    cout<<"Response: "<<response.toString()<<endl;
    return serialize(response);
}

string Server::createResponse(Status status, string description){
    Response response(status, description);
    cout<<"Response: "<<response.toString()<<endl;
    return serialize(response);
}

string Server::createResponse(Status status, vector<string>& parameters){
    Response response(status, "", parameters);
    cout<<"Response: "<<response.toString()<<endl;
    return serialize(response);
}

string Server::createResponse(Status status, string sessionId, History* history){
    LoginResponse response(status, sessionId);
    if(history!=NULL){
        response.setHistory(history);
    }
    cout<<"Login response: "<<response.toString2()<<endl;
    return serialize(response);
}

template<typename T> string Server::serialize(T& t)
{
    std::ostringstream archive_stream;
    boost::archive::text_oarchive archive(archive_stream);
    archive << t;
    return archive_stream.str();
}

template<typename T> void Server::deserialize(T& t, string serializedData)
{
    std::istringstream archive_stream(serializedData);
    boost::archive::text_iarchive archive(archive_stream);
    archive >> t;
}

string Server::generateSessionId()
{
    stringstream generatorStream;
    string signs = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for(int i=0; i<SESSION_ID_LENGTH; ++i){
        generatorStream << signs[rand() % signs.length()];
    }
    return generatorStream.str();
}
