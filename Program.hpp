#include <sys/types.h>
#include <sys/socket.h>
//#include <unordered_map>
#include <map>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "utils.hpp"
#include <fcntl.h>

class ArgvParse
{
public:
    ArgvParse(const char *cmd);
    ~ArgvParse();
    char *const *get();
private:
    void parse_cmd();
private:
    std::string argv;    //use as argv
    std::string cmd;
    std::vector<std::string> v_argv;
};


class Program
{
public:
    enum State
    {
        RUNNING=1,
        STOPPED,
        STOPPING,
        STARTING,
        BACKOFF,
        FATAL,
        EXITED,
        UNKNOWN,
    };
public:
    Program(const char *cmd);
    ~Program();
    void stop();
    void start();
    State get_state();
    void update_state(State s);
    int get_pid();
private:
    void parse_cmd();
private:
    State state;
    int pid;

    ArgvParse  argv;
};

