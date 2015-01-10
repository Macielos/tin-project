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
