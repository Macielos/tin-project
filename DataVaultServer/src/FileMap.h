#include <string>
#include <vector>

using namespace std;

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
        vector<string> files;

};
