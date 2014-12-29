/**
 *  ### ClientInterface.cpp ###
 *
 *      Ciało klasy ClientInterface.
 *
 */
#include "ClientInterface.h"

ClientInterface::ClientInterface()
{

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
            client.setHost(value);
            cout << "Wczytano parametr: host: " << value << "\n";
            break;
        }
        case MESSAGE_PORT:
        {
            short messagePort = atoi(value.c_str());
            client.setMessagePort(messagePort);
            cout << "Wczytano parametr: port poleceń: " << messagePort << "\n";
            break;
        }
        case DATA_PORT:
        {
            short dataPort = atoi(value.c_str());
            client.setDataPort(dataPort);
            cout << "Wczytano parametr: port danych: " << dataPort << "\n";
            break;
        }
    }
}

/**
 *  Oczekuje na wpisanie komendy przez użytkownika, a następnie przekazuje ją do interpretacji.
 */
 int ClientInterface::getCommand()
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
int ClientInterface::interpretCommand(string commandLine)
{
    splitCommandToWords(commandLine);
    string c = command[0]; // pierwsze słowo - jakie działanie
    command.erase(command.begin()); // usunięcie z vectora pierwszego wyrazu (bo dalej niepotrzebny)

    /*
        Komendy związane z pracą samego programu.
    */
    // EXIT
    if (c == "help")
    {
        showHelp();
        return true;
    }
    else if ( (c == "quit") || (c == "exit") )
    {
        return false;
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
    // GIVE_ACCESS
    else if (c == "give_access") // bez spacji
    {
        followTaskOnServer(GIVE_ACCESS);
    }
    else if (c == "give") // ze spacją
    {
        if (command[0] == "access")
        {
            command.erase(command.begin()); // usunięcie z vectora pierwszego wyrazu (bo dalej niepotrzebny)
            followTaskOnServer(GIVE_ACCESS);
        }
        else
        {
            cout << "Nie rozpoznano polecenia.";
        }
    }
    // REVOKE_ACCESS
    else if (c == "revoke_access") // bez spacji
    {
        followTaskOnServer(REVOKE_ACCESS);
    }
    else if (c == "revoke") // ze spacją
    {
        if (command[0] == "access")
        {
            command.erase(command.begin()); // usunięcie z vectora pierwszego wyrazu (bo dalej niepotrzebny)
            followTaskOnServer(REVOKE_ACCESS);
        }
        else
        {
            cout << "Nie rozpoznano polecenia.";
        }
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
    string login = "xyz";
    Message message(login, action, command);

    string response = sendMessage(message);
    cout << "Odpowiedź: " << response << endl;
}

/**
 *  Inicjalizuje połączenie klienta z serwerem.
 */
void ClientInterface::init()
{
    client.init();
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
