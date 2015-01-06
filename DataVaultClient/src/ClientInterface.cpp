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
unsigned int ClientInterface::checkFilenamesCorectness()
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
 *  Sprawdza czy wpisana komenda rejesteracji (jej parametry są prawidłowe).
 */
bool ClientInterface::checkRegisterCommandCorectness()
{
    if (command.size() >= 3)
    {
        if (command[0].length() < 3)
        {
            cout << "# BŁĄD: Nazwa użytkownika musi składać się przynajmniej z 3 znaków!\n";
            return false;
        }
        else if (command[1] != command[2])
        {
            cout << "# BŁĄD: Podane hasła różnią się!\n";
            return false;
        }
        else if ( (command[1].length() == 0) || (command[2].length() == 0) )
        {
            cout << "# BŁĄD: Hasło musi składać się przynajmniej z 1 znaku!\n";
            return false;
        }
    }
    else
    {
        cout << "# BŁĄD: Komenda niepełna! Poprawna składnia: [nazwa użytkownika] [hasło] [powtórzone hasło]\n";
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
            cout << "# BŁĄD: Nie jesteś zalogowany aby móc wykonać tę akcję! Użyj polecenia login [nazwa użytkownika] [hasło]\n";
        }
        else
        {
            if (action == UPLOAD)
            {
                checkFilenamesCorectness();
                if (command.size() == 0)
                {
                    return;
                }
            }
            else if (action == REGISTER)
            {
                if (checkRegisterCommandCorectness()) // jeśli komenda register poprawna
                {
                    command[1] = md5(command[1]); // zmieniamy hasło na jego hash
                    command.resize(2); // zostawiamy tylko login i hash
                }
                else
                {
                    return;
                }
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
                    cout << "Poprawnie zalogowano do serwera " << ".\n";
                    break;
                }
                case LOGOUT:
                {
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
                    // tu trzeba będzie wylistować ładnie
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
                case REMOVE:
                {
                    cout << "Plik " << "[nazwa pliku]" << " został usunięty z serwera.\n";
                    break;
                }
                case RENAME:
                {
                    cout << "Pomyślnie zmieniono nazwę pliku na serwera.\n";
                    break;
                }
                case GIVE_ACCESS:
                {
                    cout << "Przyznano uprawnienia dostępu do pliku " << " dla użytkownika " << "[login]" << ".\n";
                    break;
                }
                case REVOKE_ACCESS:
                {
                    cout << "Uprawnienia dostępu do tego pliku zostały cofnięte użytkownikowi " << "[login]" << ".n";
                    break;
                }
            }
            break;
        }
        case WRONG_SYNTAX: // co tu ma być? kiedy jest taki response?!
        {
            cout << "# BŁĄD: Wrong SYNTAX error.\n";
            break;
        }
        case FILE_EXISTS:
        {
            cout << "Plik o podanej nazwie istnieje już na serwerze.\n";
            break;
        }
        case NO_SUCH_FILE:
        {
            cout << "Plik o podanej nazwie nie istnieje na serwerze.\n";
            break;
        }
        case ACCESS_DENIED:
        {
            cout << "Nie masz dostępu do tego pliku.\n";
            break;
        }
        case INCORRECT_LOGIN:
        {
            switch (action)
            {
                case LOGIN:
                {
                    cout << "# BŁĄD: Podana nazwa użytkownika nie istnieje!\n";
                    break;
                }
                case REGISTER:
                {
                    cout << "# BŁĄD: Podana nazwa użytkownika jest zajęta!\n";
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
