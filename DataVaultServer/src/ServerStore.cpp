#include "ServerStore.h"

using namespace std;

typedef map<string, User>::iterator it_type;

ServerStore::ServerStore()
{
    prepareFileStructure();
}

ServerStore::~ServerStore()
{

}

void ServerStore::prepareFileStructure()
{
    mkdir("users/", 777);
    mkdir("admin/", 777);
    mkdir("admin/loginData/", 777);

    ofstream file;
    file.open("admin/userAccounts.txt");
    file.close();
}

vector<string> ServerStore::list(string username)
{
    return users[username].getFilelist();
}

vector<string> ServerStore::listShared(string username)
{
    return users[username].getSharedFilelist();
}

int ServerStore::add(string username, string filename)
{
    // jesli usera nie ma w bazie
    if(users.find(username) == users.end())
        return -1;
    return users[username].add(filename);
}

int ServerStore::remove(string username, string filename)
{
     // jesli usera nie ma w bazie
    if(users.find(username) == users.end())
        return -1;
    return users[username].remove(filename);
}

int ServerStore::rename(string username, string oldname, string newname)
{
    int result;
    int accessResult;
    // jesli usera nie ma w bazie
    if(users.find(username) == users.end())
        return -1;

    vector<string> usersWithAccess;
    for(it_type it = users.begin(); it != users.end(); ++it)
    {
        if(it->first!=username && it->second.hasAccess(users[username].getFile(oldname))){
            usersWithAccess.push_back(it->first);
            accessResult = revokeAccess(username, it->first, oldname);
            if(accessResult!=0){
                cerr<<"Error revoking access to "<<oldname<<endl;
            }
        }
    }

    cout<<"Users with access: "<<usersWithAccess.size()<<endl;
    result = users[username].rename(oldname, newname);

    for(vector<string>::iterator it = usersWithAccess.begin(); it != usersWithAccess.end(); ++it)
    {
        accessResult = giveAccess(username, *it, newname);
        if(accessResult!=0){
                cerr<<"Error granting access to "<<newname<<endl;
        }
    }

    return result;
}

int ServerStore::giveAccess(string fileOwner, string requestTarget, string filename)
{
    if(users.find(fileOwner) == users.end())
        return -1;
    if(users.find(requestTarget) == users.end())
        return -3;
    File* file = users[fileOwner].getFile(filename);
    if(file==NULL)
        return -2;
    return users[requestTarget].giveAccess(file);
}

int ServerStore::revokeAccess(string fileOwner, string requestTarget, string filename)
{
    if(users.find(fileOwner) == users.end())
        return -1;
    if(users.find(requestTarget) == users.end())
        return -3;
    File* file = users[fileOwner].getFile(filename);
    if(file==NULL)
        return -2;
    return users[requestTarget].revokeAccess(file);
}

bool ServerStore::hasAccess(string username, string fileOwner, string filename)
{
    if(users.find(username) == users.end())
        return false;
    if(users.find(fileOwner) == users.end())
        return false;
    File* file = users[fileOwner].getFile(filename);
    if(file==NULL)
        return false;
    return users[username].hasAccess(file);

}

int ServerStore::registerUser(string username, string hash)
{
    //ustawiamy nazwe pliku jako nazwa_uzytkownika.txt w folderze loginData
    string filename = "admin/loginData/" + username + ".txt";

    //sprawdzenie czy nie ma juz uzytkownika o takiej nazwie
    if(loginFileExists(filename))
    {
        return -1;
    }

    if(userExists(username)){
        return -1;
    }

    //stworzenie katalogu
    string path = "users/" + username;
    const char* nameOfPath = path.c_str();

    mkdir(nameOfPath, 777);

    const char* nameOfFile = filename.c_str();

    ofstream file;
    file.open(nameOfFile);
    file << username << endl << hash;
    file.close();

    //dodanie do userAccounts
    nameOfFile = "admin/userAccounts.txt";
    file.open(nameOfFile, ios::app);
    file << username << endl;
    file.close();

    User user;
    user.setUsername(username);
    user.setPasswordHash(hash);
    users.insert(make_pair(username, user));
    return 0;
    // jesli username nie ma w bazie (można rejestrować)
}

int ServerStore::unregisterUser(string username, string hash)
{
    //ustawiamy nazwe pliku jako nazwa_uzytkownika.txt w folderze loginData
    string filename = "admin/loginData/" + username + ".txt";
    const char* nameOfFile = filename.c_str();
    string fileUsername, filePasshash;

    //sprawdzenie czy taki plik w ogole istnieje
    if(!loginFileExists(filename))
    {
        return -1;
    }

    ifstream read(nameOfFile);
    getline(read, fileUsername);
    getline(read, filePasshash);

    if(fileUsername == username)
    {
        if(filePasshash == hash)
        {
            //usuwanie pliku z danymi o logowaniu

            vector<string> filenames = list(username);
            for(unsigned int i=0; i<filenames.size(); ++i)
            {
                std::remove(filenames[i].c_str());
            }
            users[username].deleteFiles();
            std::remove(nameOfFile);

            //usuniecie katalogów
            string path = "users/" + username;
            const char* nameOfPath = path.c_str();

            rmdir(nameOfPath);

            string path2 = "admin/loginData/" + username;
            const char* nameOfPath2 = path.c_str();

            rmdir(nameOfPath2);

            //usuniecie z userAccounts
            string line;
            ifstream file;
            nameOfFile = "admin/userAccounts.txt";
            file.open(nameOfFile);
            ofstream outfile;
            const char* nameOfOutfile = "admin/userAccountsTemp.txt";
            file.open(nameOfOutfile);

            while(getline(file,line))
            {
                if(line==username){}
                else {outfile<<line<<endl;}
            }
            outfile.close();
            file.close();

            users.erase(username);
            return 0;

        }
        else
        {
            return -2;
        }
    }
    else
    {
        return -1;
    }
}

int ServerStore::loginUser(string username, string hash)
{
    //ustawiamy nazwe pliku jako nazwa_uzytkownika.txt w folderze loginData
    string filename = "admin/loginData/" + username + ".txt";
    const char* nameOfFile = filename.c_str();
    string fileUsername, filePasshash;

    //sprawdzenie czy taki plik w ogole istnieje
    if(!loginFileExists(filename))
    {
        return -1;
    }

    ifstream read(nameOfFile);
    getline(read, fileUsername);
    getline(read, filePasshash);

    if(fileUsername == username)
    {
        if(filePasshash == hash)
        {
            //poprawna nazwa uzytkownika i haslo
            return 0;
        }
        else
        {   // niepoprawne haslo
            return -2;
        }
    }
    else
    {   // niepoprawny login
        return -1;
    }
}

string ServerStore::getSessionId(string username)
{
    if(users.find(username) == users.end())
        return "";
    return users[username].getSessionId();
}

void ServerStore::setSessionId(string username, string sessionId)
{
    if(users.find(username) == users.end())
        return;
    users[username].setSessionId(sessionId);
}

bool ServerStore::fileExists(string username, string filename)
{
    if(users.find(username) == users.end())
        return false;
    return users[username].fileExists(filename);
}

bool ServerStore::userExists(string username)
{
    return users.find(username) != users.end();
}

bool ServerStore::loginFileExists(string filename) {
    ifstream f(filename.c_str());
    if (f.good()) {
        f.close();
        return true;
    } else {
        f.close();
        return false;
    }
}

History* ServerStore::getHistory(string username)
{
    if(users.find(username) == users.end())
        return NULL;
    return users[username].getHistory();
}

int ServerStore::clearHistory(string username)
{
    if(users.find(username) == users.end())
        return -1;
    users[username].clearHistory();
    return 0;
}

void ServerStore::clearAllHistory()
{
    for(it_type it = users.begin(); it != users.end(); ++it)
    {
        it->second.clearHistory();
    }
}

void ServerStore::updateHistory(EventType type, string fileOwner, string filename)
{
    if(type==FILE_RENAMED || type==ACCESS_GRANTED || type==ACCESS_REVOKED){
        cerr<<"Próba dodania do historii niepoprawnego eventu o typie "<<type<<endl;
        return;
    }
    updateHistory(type, fileOwner, filename, "");
}

/**
 *  Parametr secondary:
 *  FILE_MODIFIED/REMOVED: secondary nieużywany
 *  FILE_RENAMED: secondary - stara nazwa pliku
 *  ACCESS_GRANTED/REVOKED: secondary - użytkownik, któremu przyznano/odebrano dostęp
 */
void ServerStore::updateHistory(EventType type, string fileOwner, string filename, string secondary)
{
    File* file = users[fileOwner].getFile(filename);
    if(file==NULL){
        cerr<<"Błąd dodawania zdarzenia: nie znaleziono pliku "<<filename<<endl;
        return;
    }
    switch(type){
    case FILE_MODIFIED: case FILE_REMOVED: case FILE_RENAMED:
        for(it_type it = users.begin(); it != users.end(); ++it)
        {
            if(it->first!=fileOwner && it->second.hasAccess(file)){
                Event event(filename, fileOwner, secondary);
                it->second.addEvent(type, event);
                cout<<"Dodano zdarzenie: "<<endl<<event.toString();
            }
        }
        break;
    case ACCESS_GRANTED: case ACCESS_REVOKED:
        Event event(filename, fileOwner);
        users[secondary].addEvent(type, event);
        cout<<"Dodano zdarzenie: "<<endl<<event.toString();
        break;
    }

}
