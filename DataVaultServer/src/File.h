#include <string>
#include <map>

using namespace std;

class File
{
        string filename;
        string owner;
    public:
        File();
        File(string filename, string owner);
        ~File();
        string getFilename();
        string getOwner();
        void setFilename(string filename);
        void setOwner(string owner);

};
