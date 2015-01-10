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
        void setFilename(string filename);

    private:
        string filename;
};
