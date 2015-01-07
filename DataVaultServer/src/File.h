#include <string>
#include <map>

using namespace std;

class File
{
    public:
        File();
        File(string filename);
        ~File();
        string getFilename();
        int setFilename(string filename);

    private:
        string filename;
};
/*
class FileMap
{
    public:
        FileMap();
        ~FileMap();
        vector<string> getFilelist();
        int add(string filename);
        int remove(string filename);
        int rename(string oldname, string newname);

    private:
        map<string> files;

};
*/
