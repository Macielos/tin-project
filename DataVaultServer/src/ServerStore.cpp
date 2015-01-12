#include "ServerStore.h"

using namespace std;

ServerStore::ServerStore()
{
    User user;
    user.setUsername("user123");
    users.push_back(user);
}

ServerStore::~ServerStore()
{

}

vector<string>& ServerStore::list(string username)
{
    // Przejscie po liscie wszystkich userow
    for(vector<User>::iterator it = users.begin(); it != users.end(); ++it)
    {
        // jeśli znaleziono usera
        if(it->getUsername() == username)
            return it->getFilelist();
    }
    return noUser;
}

int ServerStore::add(string username, string filename)
{
    // Przejscie po liscie wszystkich userow
    for(vector<User>::iterator it = users.begin(); it != users.end(); ++it)
    {
        // jeśli znaleziono usera
        if(it->getUsername() == username)
            return it->add(filename);
    }
    // jesli usera nie ma w bazie
    return -1;
}

int ServerStore::remove(string username, string filename)
{
    // Przejscie po liscie wszystkich userow
    for(vector<User>::iterator it = users.begin(); it != users.end(); ++it)
    {
        // jeśli znaleziono usera
        if(it->getUsername() == username)
            return it->remove(filename);
    }
    // jesli usera nie ma w bazie
    return -1;
}

int ServerStore::rename(string username, string oldname, string newname)
{
    // Przejscie po liscie wszystkich userow
    for(vector<User>::iterator it = users.begin(); it != users.end(); ++it)
    {
        // jeśli znaleziono usera
        if(it->getUsername() == username)
            return it->rename(oldname, newname);
    }
    // jesli usera nie ma w bazie
    return -1;
}

int ServerStore::registerUser(string username, string hash)
{
      //ustawiamy nazwe pliku jako nazwa_uzytkownika.txt w folderze loginData
      string filename = "loginData/" + username + ".txt";

      //sprawdzenie czy nie ma juz uzytkownika o takiej nazwie
      if(loginFileExists(filename))
        {
            return -1;
        }

      const char* nameOfFile = filename.c_str();

      ofstream file;
      file.open(nameOfFile);
      file << username << endl << hash;
      file.close();

    // Przejscie po liscie wszystkich userow
    for(vector<User>::iterator it = users.begin(); it != users.end(); ++it)
    {
        // jeśli znaleziono taki username
        if(it->getUsername() == username)
            return -1;
    }
    User user;
    user.setUsername(username);
    user.setHash(hash);
    users.push_back(user);
    return 0;
    // jesli username nie ma w bazie (można rejestrować)
}

int ServerStore::unregisterUser(string username, string hash)
{
   /* // Przejscie po liscie wszystkich userow
    for(vector<User>::iterator it = users.begin(); it != users.end(); ++it)
    {
        // jeśli znaleziono taki username
        if(it->getUsername() == username)
        {
            if (it->getHash() == hash)
            {
                users.erase(it);
                return 0;
            }
            else
            {
                return -2;
            }
        }
    }
    return -1;   */

      //ustawiamy nazwe pliku jako nazwa_uzytkownika.txt w folderze loginData
      string filename = "loginData/" + username + ".txt";
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
                    std::remove(nameOfFile);
                    return 0;

                 /*
                !!! WAŻNE !!!

                TUTAJ (przed usunięciem usera)

                wstawić usuwanie wszystkich jego plików
                wstawić usuwanie jesgo katalogu
                */

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
      string filename = "loginData/" + username + ".txt";
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

bool ServerStore::fileExists(string username, string filename)
{
    for(vector<User>::iterator it = users.begin(); it != users.end(); ++it)
    {
        // jeśli znaleziono usera
        if(it->getUsername() == username)
        {
            return it->fileExists(filename);
        }
    }
    return false;
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
