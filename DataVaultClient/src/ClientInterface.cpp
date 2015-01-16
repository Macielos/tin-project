/**
 *  ### ClientInterface.cpp ###
 *
 *      Ciało klasy ClientInterface.
 *
 */
#include "ClientInterface.h"

ClientInterface::ClientInterface()
{
    cout << "\n\t# DATA VAULT #\n\n"; // powitanie
}

ClientInterface::~ClientInterface()
{

}

/**
 *  Ustawia klientowi konkretny parametr połączenia pobrany od użytkownika.
 */
void ClientInterface::changeParameter(ConnectionParameter param, string value)
{
    switch (param)
    {
        case HOST:
        {
            if (client.setHost(value))
            {
                cout << "Wczytano parametr: host: " << value << "\n";
            }
            else
            {
                cout << "# BŁĄD: Niepoprawna nazwa hosta!\n";
            }
            break;
        }
        case MESSAGE_PORT:
        {
            int messagePort = atoi(value.c_str());
            if (client.setMessagePort(messagePort))
            {
                cout << "Wczytano parametr: port poleceń: " << messagePort << "\n";
            }
            else
            {
                cout << "# BŁĄD: Niepoprawny numer portu poleceń!\n";
            }
            break;
        }
        case DATA_PORT:
        {
            int dataPort = atoi(value.c_str());
            if (client.setDataPort(dataPort))
            {
                cout << "Wczytano parametr: port danych: " << dataPort << "\n";
            }
            else
            {
                cout << "# BŁĄD: Niepoprawny numer portu danych!\n";
            }
            break;
        }
        case NOTIFICATION_PORT:
        {
            int notificationPort = atoi(value.c_str());
            if (client.setNotificationPort(notificationPort))
            {
                cout << "Wczytano parametr: port powiadomień: " << notificationPort << "\n";
            }
            else
            {
                cout << "# BŁĄD: Niepoprawny numer portu powiadomień!\n";
            }
            break;
        }
    }
}

/**
 *  Oczekuje na wpisanie komendy przez użytkownika, a następnie przekazuje ją do interpretacji.
 */
bool ClientInterface::getCommand()
{
    string s;
    command.clear();
    cout << "\n> ";
    getline(cin, s);
    return interpretCommand(s);
}

/**
 *  Interpretuje komendę wpisaną przez użytkownika. Zwracana wartość 0 - koniec działania programu.
 */
bool ClientInterface::interpretCommand(string commandLine)
{
    splitCommandToWords(commandLine); // dzielenie komendy na wyrazy

    string c = command[0]; // pierwsze słowo - jakie działanie
    transform(c.begin(), c.end(), c.begin(), ::tolower); // wszystkie litery małe
    command.erase(command.begin()); // usunięcie z vectora pierwszego wyrazu (bo dalej niepotrzebny)

    /*
        Komendy związane z pracą samego programu.
    */
    // pusta komenda
    if (c.length() == 0)
    {
        return true;
    }
    // EXIT
    else if ( (c == "quit") || (c == "exit") )
    {
        return false;
    }
    // HELP
    else if (c == "help")
    {
        showHelp();
    }
    // CONNECT
    else if (c == "connect")
    {
        connect();
    }
    // SET...
    else if (c == "set")
    {
        string c_t = command[0]; // c_tymczsowe
        transform(c_t.begin(), c_t.end(), c_t.begin(), ::tolower); // wszystkie litery małe
        // ...HOST
        if (c_t == "host")
        {
            changeParameter(HOST, command[1]);
        }
        // ...MESSAGE_PORT
        else if ( (c_t == "message_port") || ( c_t == "portp") || ( c_t == "mport") || ( c_t == "messageport") )
        {
            changeParameter(MESSAGE_PORT, command[1]);
        }
        // ...DATA_PORT
        else if ( (c_t == "data_port") || ( c_t == "portd") || ( c_t == "dport") || ( c_t == "dataport") )
        {
            changeParameter(DATA_PORT, command[1]);
        }
        // ...NOTIFICATION_PORT
        else if ( (c_t == "notification_port") || ( c_t == "portn") || ( c_t == "nport") || ( c_t == "notificationport") )
        {
            changeParameter(NOTIFICATION_PORT, command[1]);
        }
        else
        {
            cout << "# BŁĄD: Nie rozpoznano parametru '" << c_t << "'!\n";
        }
    }
    /*
        Komendy związane z poleceniami do serwera.
    */
    // REGISTER
    else if (c == "register")
    {
        followTaskOnServer(REGISTER);
    }
    // LOGIN
    else if (c == "login")
    {
        followTaskOnServer(LOGIN);
    }
    // LOGOUT
    else if (c == "logout")
    {
        followTaskOnServer(LOGOUT);
    }
    // UNREGISTER
    else if (c == "unregister")
    {
        followTaskOnServer(UNREGISTER);
    }
    // LIST
    else if (c == "list")
    {
        followTaskOnServer(LIST);
    }
    // UPLOAD
    else if (c == "upload")
    {
        followTaskOnServer(UPLOAD);
    }
    // DOWNLOAD
    else if (c == "download")
    {
        followTaskOnServer(DOWNLOAD);
    }
    // DOWNLOAD SHARED
    else if (c == "download_shared")
    {
        followTaskOnServer(DOWNLOAD_SHARED);
    }
    // REMOVE
    else if (c == "remove")
    {
        followTaskOnServer(REMOVE);
    }
    // RENAME
    else if (c == "rename")
    {
        followTaskOnServer(RENAME);
    }
    // GIVE_ACCESS bez spacji
    else if (c == "give_access")
    {
        followTaskOnServer(GIVE_ACCESS);
    }
    // REVOKE_ACCESS bez spacji
    else if (c == "revoke_access")
    {
        followTaskOnServer(REVOKE_ACCESS);
    }
    // GIVE_ACCESS lub REVOKE ACCESS ze spacją
    else if ( (c == "give") || (c == "revoke") )
    {
        string c_t = command[0]; // c_tymczsowe
        transform(c_t.begin(), c_t.end(), c_t.begin(), ::tolower); // wszystkie litery małe
        if (c_t == "access") // jeżeli drugie słowo ok
        {
            command.erase(command.begin()); // usunięcie z vectora pierwszego wyrazu (bo dalej niepotrzebny)
            followTaskOnServer((c == "give") ? GIVE_ACCESS : REVOKE_ACCESS);
        }
        else
        {
            cout << "# BŁĄD: Nie rozpoznano polecenia! Czy chodziło ci o '" << c << " access'?\n";
        }
    }
    else
    {
        cout << "# BŁĄD: Nie rozpoznano polecenia!\n";
    }

    return true;
}

/**
 *  Odpowiada za podział stringa na wyrazy, które zostaną wpisane do vectora command.
 */
void ClientInterface::splitCommandToWords(string commandLine)
{
    command.clear();
    boost::algorithm::split(command, commandLine, boost::algorithm::is_any_of(" "));
}

/**
 *  Sprawdza istnienie pliku na dysku.
 */
inline bool ClientInterface::checkFileExist (const string& name)
{
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}

/**
 *  Sprawdza czy nazwy plików (jako parametry w wektorze command) są poprawne (istnieją).
 *  Kiedy plik nie będzie istniał wypisze komunikat i usunie nazwę z wektora i będzie kontynuował sprawdzanie.
 *  Zwraca ilość niepoprawnych nazw.
 */
unsigned int ClientInterface::checkFilenamesCorrectness()
{
    unsigned int incorrect = 0;
    for (unsigned int i = 0; i < command.size(); i++) // pętla po nazwach plików
    {
        if (command[i].length() == 0)
        {
            command.erase(command.begin() + i);
            i--;
        }
        else if (!checkFileExist(command[i])) // jeśli przetwarzany plik nie istnieje
        {
            cout << "# BŁĄD: Nie można odnaleźć pliku o nazwie \"" << command[i] << "\"!\n";
            command.erase(command.begin() + i);
            incorrect++;
            i--;
        }
    }
    return incorrect;
}

/**
 *  Sprawdza czy wpisana komenda wyrejestrowania jest ok (jej parametry są prawidłowe).
 */
bool ClientInterface::checkCommandCorrectness()
{
    if (command.size() >= 2)
    {
        if (command[0].length() < 3)
        {
            cout << "# BŁĄD: Nazwa użytkownika musi składać się przynajmniej z 3 znaków!\n";
            return false;
        }
        else if ( command[1].length() == 0 )
        {
            cout << "# BŁĄD: Hasło musi składać się przynajmniej z 1 znaku!\n";
            return false;
        }
    }
    else
    {
        cout << "# BŁĄD: Komenda niepełna! Poprawna składnia: [nazwa użytkownika] [hasło] \n";
        return false;
    }
    return true;
}

/**
 *  Odpowiada za przygotowanie i wysłanie żądania do serwera.
 */
void ClientInterface::followTaskOnServer(Action action)
{
    if (client.isConnected())
    {
        // jeśli klient nie jest zalogowany (uwierzytelniony) i akcja nie dotyczy rejestracji / logowania
        if ( !client.isLogged()
                && action != REGISTER
                && action != LOGIN
                && action != UNREGISTER )
        {
            cout << "# BŁĄD: Musisz być zalogowany, aby móc wykonać tę akcję! Użyj polecenia login [nazwa użytkownika] [hasło]\n";
        }
        else if ( action == REGISTER && client.isLogged() )
        {
            cout << "# BŁĄD: Wyloguj się, aby zarejestrowac nowego użytkownika\n";
        }
        else if ( action == LOGIN && client.isLogged() )
        {
            cout << "# BŁĄD: Jesteś juz zalogowany. Aby zalogowac innego uzytkownika, najpierw wyloguj się.\n";
        }
        else
        {
            if (action == UPLOAD)
            {
                checkFilenamesCorrectness();
                if (command.size() == 0)
                {
                    return;
                }
            }
            else if (action == REGISTER || action == UNREGISTER || action == LOGIN)
            {
                    if (checkCommandCorrectness())
                    {
                        command[1] = md5(command[1]); // zmieniamy hasło na jego hash
                        command.resize(2); // zostawiamy tylko login i hash
                    }
                    else
                    {
                        return;
                    }
            }
            else if (action == LOGOUT)
            {
                if(!client.isLogged())
                {
                    cout << "Polecenie dziala po zalogowaniu.\n";
                    return;
                }
            }

            if(action == REGISTER || action == LOGIN){
                client.setUserId(command[0]);
            }

            Message message(action, command);
            Response* response = client.sendMessage(message);

            processResponse(action, response, message);

            delete response;
        }
    }
    else
    {
        cout << "# BŁĄD: Brak połączenia z serwerem! Użyj polecenia: connect\n";
    }
}

/**
 *  Przetwarzanie odpowiedzi z serwera (response).
 */
void ClientInterface::processResponse(Action action, Response* response, Message& message)
{
    switch (response->getStatus())
    {
        case OK:
        {
            switch (action)
            {
                case REGISTER:
                {
                    cout << "Rejestracja zakończona sukcesem. Możesz teraz zalogować się używając polecenia login.\n";
                    break;
                }
                case LOGIN:
                {
                    LoginResponse* loginResponse = (LoginResponse*) response;
                    if(loginResponse!=NULL)
                    {
                        client.setLogged(true);
                        cout << "Poprawnie zalogowano do serwera.\n";
                        int historySize = loginResponse->getHistory().size();
                        cout << "Nowe powiadomienia: "<<historySize<<endl;
                        if(historySize>0)
                        {
                            printHistory(loginResponse->getHistory());
                        }
                    }else{
                        cout << "# BŁĄD: Po stronie serwera wystąpił błąd podczas logowania.\n";
                    }

                    break;
                }
                case LOGOUT:
                {
                    client.setLogged(false);
                    cout << "Wylogowano z serwera.\n";
                    break;
                }
                case UNREGISTER:
                {
                    cout << "Nastąpiło poprawne wyrejestrowanie użytkownika z bazy danych serwera. Przechowywane pliki zostały nieodwracalnie usunięte.\n";
                    break;
                }
                case LIST:
                {
                    cout<<"Twoje pliki na serwerze: "<<response->getParameters().size()<<endl;
                    for(unsigned int i=0; i<response->getParameters().size(); ++i){
                        cout<<"  "<<response->getParameters()[i]<<endl;
                    }
                    break;
                }
                case UPLOAD:
                {
                    bool result;
                    for (unsigned int i = 0; i < message.getParameters().size(); ++i)
                    {
                        if (!checkFileExist(message.getParameters()[i])) // jeśli przetwarzany plik nie istnieje
                        {
                            break;
                        }
                        cout << "Przesyłanie pliku \"" << message.getParameters()[i] << "\"...\n";
                        result = client.sendFile(message.getParameters()[i], i != 0);
                        if(!result)
                        {
                            cout << "# BŁĄD: Nie udało się wysłać pliku na serwer!\n";
                            break;
                        }
                    }
                    break;
                }
                case DOWNLOAD:
                {
                    bool result;
                    for (unsigned int i = 0; i < message.getParameters().size(); ++i)
                    {
                        cout << "Pobieranie pliku " << message.getParameters()[i] << "...\n";
                        result = client.receiveFile(message.getParameters()[i], true);
                        if (!result)
                        {
                            cout << "# BŁĄD: Nie udało się pobrać pliku!\n";
                            break;
                        }
                    }
                    break;
                }
                case DOWNLOAD_SHARED:
                {
                    bool result;
                    for (unsigned int i = 1; i < message.getParameters().size(); ++i)
                    {
                        cout << "Pobieranie udostępnionego pliku " << message.getParameters()[i] << "...\n";
                        result = client.receiveFile(message.getParameters()[i], true);
                        if (!result)
                        {
                            cout << "# BŁĄD: Nie udało się pobrać pliku!\n";
                            break;
                        }
                    }
                    break;
                }
                case REMOVE:
                {
                    cout << "Pliki zostały usunięte z serwera.\n";
                    break;
                }
                case RENAME:
                {
                    cout << "Pomyślnie zmieniono nazwę pliku na serwerze.\n";
                    break;
                }
                case GIVE_ACCESS:
                {
                    cout << "Przyznano uprawnienia dostępu do plików dla użytkownika " << message.getParameters()[0] << ".\n";
                    break;
                }
                case REVOKE_ACCESS:
                {
                    cout << "Uprawnienia dostępu do plików zostały cofnięte użytkownikowi " << message.getParameters()[0] << ".n";
                    break;
                }
            }
            break;
        }
        case WRONG_SYNTAX:
        {
            cout << "# BŁĄD: Niepoprawne polecenie lub liczba argumentów.\n";
            break;
        }
        case FILE_EXISTS:
        {
            cout << "# BŁĄD: Plik o podanej nowej nazwie istnieje już na serwerze.\n";
            break;
        }
        case NO_SUCH_FILE:
        {
            cout << "# BŁĄD: Pliki o podanych nazwach nie istnieją na serwerze.\n" << printParameters(response->getParameters()) << endl;
            break;
        }
        case NO_SUCH_USER:
        {
            cout << "# BŁĄD: Użytkownik o podanej nazwie nie jest zarejestrowany na serwerze.\n";
            break;
        }
        case ACCESS_DENIED:
        {
            cout << "# BŁĄD: Odmówiono dostępu do podanych plików.\n" << printParameters(response->getParameters()) << endl;
            break;
        }
        case ALREADY_HAVE_ACCESS:
        {
            cout << "# BŁĄD: Działanie bez efektu. Użytkownik ma już dostęp do podanych plików.\n" << printParameters(response->getParameters()) << endl;
            break;
        }
        case ALREADY_NO_ACCESS:
        {
            cout << "# BŁĄD: Działanie bez efektu. Użytkownik nie ma dostępu do podanych plików.\n" << printParameters(response->getParameters()) << endl;
            break;
        }
        case OWN_FILE:
        {
            cout << "# BŁĄD: Próba przyznania/odebrania samemu sobie dostępu do własnego pliku. Interesująca, acz pozbawiona sensu.\n" << printParameters(response->getParameters()) << endl;
            break;
        }
        case INCORRECT_LOGIN:
        {
            switch (action)
            {
                case REGISTER:
                {
                    cout << "# BŁĄD: Podana nazwa użytkownika jest zajęta!\n";
                    break;
                }
                default:
                {
                    cout << "# BŁĄD: Podana nazwa użytkownika nie istnieje!\n";
                    break;
                }
            }
            break;
        }
        case INCORRECT_PASSWORD:
        {
            cout << "# BŁAD: Podane hasło jest niepoprawne!\n";
            break;
        }
        case USERNAME_USED:
        {
            cout << "Wybrana nazwa użytkownika jest już zajęta.\n";
            break;
        }
        case NOT_IMPLEMENTED:
        {
            cout << "[funkcja jeszcze nie zaimplementowana na serwerze]\n";
            break;
        }
    }
}

/**
 *  Inicjalizuje połączenie klienta z serwerem.
 */
void ClientInterface::connect()
{
    if (client.isValidParameters())
    {
        cout << "\nNawiązywanie połączenia...\n";
        if (client.connect())
        {
            cout << "Połączono z serwerem.\n";
        }
        else
        {
            cout << "# BŁĄD: Nie można nawiązać połączenia z serwerem!\n";
        }
    }
    else
    {
        cout << "# BŁĄD: Brak danych połączenia! Użyj polecenia: set [parametr] [wartość]\n";
    }
}

/**
 *  Wypisuje pomoc dotyczącą poleceń programu.
 */
inline void ClientInterface::showHelp()
{
    cout << "\n\n\n\t\tDATA VAULT\n\n"
         << "  set          - zmienia ustawienia połączenia z serwerem\n\t\t\tskładnia: set [host|portp|portd|portn] [wartość]\n"
         << "  connect      - ustanawia połączenie z serwerem\n\t\t\tbez parametrów\n"
         << "  login        - loguje użytkownika do serwera przy użyciu loginu i hasła\n\t\t\tskładnia: login [nazwa użytkownika] [hasło]\n"
         << "  logout       - wylogowuje użytkownika z serwera\n\t\t\tbez parametrów\n"
         << "  register     - rejestruje użytkownika na serwerze przy użyciu loginu i hasła\n\t\t\tskładnia: register [nazwa użytkownika] [hasło] [powtórz hasło]\n"
         << "  unregister   - wyrejestrowuje użytkownika z serwera\n\t\t\tskładnia: unregister [nazwa użytkownika] [hasło]\n"
         << "  list         - pokazuje informacje o plikach\n\t\t\tskładnia: list [parametr]\n"
         << "  upload       - wysyła plik z dysku lokalnego na serwer\n\t\t\tskładnia: upload [nazwa pliku(ścieżka względem programu)]\n"
         << "  download     - pobiera plik z serwera na dysk lokalny\n\t\t\tskładnia: download [nazwa pliku na serwerze]\n"
         << "  remove       - usuwa plik z serwera\n\t\t\tskładnia: remove [nazwa pliku]\n"
         << "  rename       - zmienia nazwę pliku na serwerze\n\t\t\tskładnia: rename [nazwa pliku (stara)] [nazwa pliku (nowa)]\n"
         << "  give access  - przyznaje dostęp innemu użytkownikowi do danego pliku na serwerze\n\t\t\tskładnia: give access [nazwa pliku na serwerze] [nazwa użytkownika]\n"
         << "  revoke access- odbiera dostęp innemu użytkownikowi do danego pliku na serwerze\n\t\t\tskładnia: revoke access [nazwa pliku na serwerze] [nazwa użytkownika]\n"
         << "  help         - pokazuje tę pomoc\n"
         << "  exit/quit    - wchodzi z programu\n"
         << endl;
}

string ClientInterface::printParameters(vector<string>& parameters)
{
    stringstream ss;
    for(unsigned int i=0; i<parameters.size(); ++i){
        ss << parameters[i];
        if(i!=parameters.size()-1){
            ss << " ";
        }
    }
    return ss.str();
}

void ClientInterface::printHistory(History& history)
{
    if(history.getEvents(ACCESS_GRANTED)->size()>0){
        cout<<"Przyznano ci prawa dostępu do plików: "<<endl;
        printEvents(*history.getEvents(ACCESS_GRANTED), ACCESS_GRANTED);
    }
    if(history.getEvents(ACCESS_REVOKED)->size()>0){
        cout<<"Odebrano ci prawa dostępu do plików: "<<endl;
        printEvents(*history.getEvents(ACCESS_REVOKED), ACCESS_REVOKED);
    }
    if(history.getEvents(FILE_MODIFIED)->size()>0){
        cout<<"Zmodyfikowano śledzone przez ciebie pliki: "<<endl;
        printEvents(*history.getEvents(FILE_MODIFIED), FILE_MODIFIED);
    }
    if(history.getEvents(FILE_REMOVED)->size()>0){
        cout<<"Usunięto śledzone przez ciebie pliki: "<<endl;
        printEvents(*history.getEvents(FILE_REMOVED), FILE_REMOVED);
    }
    if(history.getEvents(FILE_RENAMED)->size()>0){
        cout<<"Zmieniono nazwę śledzonych przez ciebie plików: "<<endl;
        printEvents(*history.getEvents(FILE_RENAMED), FILE_RENAMED);
    }
}

void ClientInterface::printEvents(vector<Event>& events, EventType type)
{
    for(unsigned int i=0; i<events.size(); ++i){
        cout<<"  "<<events[i].getFilename();
        if(type==FILE_RENAMED){
            cout<<" przemianowany z "<<events[i].getSecondaryParameter();
        }
        cout<<" przez użytkownika "<<events[i].getUsername()<<endl;
    }
}
