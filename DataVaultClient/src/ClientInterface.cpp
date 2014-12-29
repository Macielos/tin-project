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
    //dtor
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
    }
}

/**
 *  Oczekuje na wpisanie komendy przez użytkownika, a następnie przekazuje ją do interpretacji.
 */
 bool ClientInterface::getCommand()
 {
    string s;
    command.clear();
    cout << "\n>";
    getline(cin, s);
    return interpretCommand(s);
 }

/**
 *  Interpertuje komendę wpisaną przez użytkownika. Zwracana wartość 0 - koniec działania programu.
 */
bool ClientInterface::interpretCommand(string commandLine)
{
    splitCommandToWords(commandLine);
    string c = command[0]; // pierwsze słowo - jakie działanie
    transform(c.begin(), c.end(), c.begin(), ::tolower); // wszystkie litery małe
    command.erase(command.begin()); // usunięcie z vectora pierwszego wyrazu (bo dalej niepotrzebny)

    /*
        Komendy związane z pracą samego programu.
    */
    // EXIT
    if ( (c == "quit") || (c == "exit") )
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
        else if ( (c_t == "message_port") || ( c_t == "mport") || ( c_t == "messageport") )
        {
            changeParameter(MESSAGE_PORT, command[1]);
        }
        // ...DATA_PORT
        else if ( (c_t == "data_port") || ( c_t == "dport") || ( c_t == "dataport") )
        {
            changeParameter(DATA_PORT, command[1]);
        }
        else
        {
            cout << "# BŁĄD: Nie rozpoznano parametru set!\n";
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
            cout << "# BŁĄD: Nie rozpoznano polecenia!\n";
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
    string s = commandLine, word = "";
    unsigned char c;

    s += " "; // dodajemy wartownika

    for(unsigned int i = 0; i < s.length(); i++)
    {
        c = s[i]; // przechodzimy po kolejnych literach
        if(((c >= '0') && (c <= '9')) || (c == '_') || (c == '-')  ||
            ((c >= 'A') && (c <= 'Z')) ||((c >= 'a') && (c <= 'z')) ||
            (c == 164) || (c == 165)  || (c == 143) || (c == 134)  ||
            (c == 168) || (c == 169)  || (c == 157) || (c == 136)  ||
            (c == 227) || (c == 228)  || (c == 224) || (c == 162)  ||
            (c == 151) || (c == 152)  || (c == 141) || (c == 171)  ||
            (c == 189) || (c == 190))
        {
            word += c;
        }
        else if(word != "")
        {
            command.push_back(word);
            word = "";
        }
    }
}

/**
 *  Odpowiada za przygotowanie i wysłanie żądania do serwera.
 */
void ClientInterface::followTaskOnServer(Action action)
{
    if (client.isConnected())
    {
        string userId = "user123";
        Message message(userId, action, command);

        string response = sendMessage(message);
        cout << "Odpowiedź: " << response << endl;
    }
    else
    {
        cout << "# BŁĄD: Brak połączenia z serwerem! Użyj polecenia: connect\n";
    }
}

/**
 *  Inicjalizuje połączenie klienta z serwerem.
 */
void ClientInterface::connect()
{
    if (client.isValidParameters())
    {
        cout << "\nNawiązywanie połączenia..." << endl;
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
 *  Wysyła polecenie do serwera.
 */
string ClientInterface::sendMessage(Message& message)
{
    return client.sendMessage(message);
}

/**
 *  Wypisuje pomoc dotyczącą poleceń programu.
 */
 void ClientInterface::showHelp()
 {
    cout << "\n\n\n\t\tDATA VAULT\n\n";
    cout << "\tlogin        - loguje użytkownika do serwera przy użyciu loginu i hasła\n\t\tprzykład użycia: login user123 pass456\n";
    cout << "\tregister     - rejestruje użytkownika na serwerze przy użyciu loginu i hasła\n\t\tprzykład użycia: register user123 pass456 pass456\n";
    cout << endl;
 }
