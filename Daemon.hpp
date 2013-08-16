#include <sys/types.h>
#include <sys/socket.h>
//#include <unordered_map>
#include <map>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#include "Program.hpp"


class Daemon
{
public:
    //typedef std::tr1::unordered_map<const char *,Program *> ProgramMap;
    typedef std::map<const char *,Program *> ProgramMap;
public:
    Daemon();
    ~Daemon();
    void stop(const char *name);
    void  start(const char *name);
    void add(const char *name,const char *cmd);

    void loop();
private:
    Program *get_program(const char *name);
    Program::State update_state_if_terminated(Program *p);
private:
    ProgramMap  v_program;
};

