#include <string>
#include <map>

using namespace std;

class File
{
        string filename;

    public:
        File();
        File(string filename);
        ~File();
        string getFilename();
        void setFilename(string filename);

};
