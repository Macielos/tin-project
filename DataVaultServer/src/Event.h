class Event
{
        long date;
        string filename;
        string username;
    public:
        Event(long date, string filename, string username);
        ~Event();

        long getDate();
        string getFilename();
        string getUsername();
};
