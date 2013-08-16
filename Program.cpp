#include "Program.hpp"

ArgvParse::~ArgvParse()
{
}

ArgvParse::ArgvParse(const char *cmd)
    :cmd(cmd)
{
    parse_cmd();
}

char *const *ArgvParse::get()
{
    return (char *const *)argv.c_str();
}

void ArgvParse::parse_cmd()
{
    const char *s=cmd.c_str();

    int len;
    while (true)
    {
        len=supper_strchr(&s,' ');
        if (len>0) 
        {
            std::string tmp(s,len);
            v_argv.push_back(tmp);
            printf("(%s:%d) '%s' \n",__func__,__LINE__,tmp.c_str());
        }
        else
        {
            break;
        }
        s+=len;
    }

    int size=v_argv.size();

#ifndef ARG_MAX
#define ARG_MAX (sysconf(_SC_ARG_MAX))
#endif
    if (ARG_MAX<size)
    {
        throw "argument too long...";
    }
   
    argv.resize((sizeof(char *)*(size+1)),0);
    const char **buf=(const char **)argv.c_str();
    for(int i=0;i<size;++i)
    {
        buf[i]=v_argv[i].c_str();
    }
    buf[size]=NULL;
}


Program::Program(const char *cmd)
    :pid(-1),state(STOPPED),argv(cmd)
{
}

Program::~Program()
{

}



void Program::update_state(State s)
{
    state=s;
}

int  Program::get_pid()
{
    return pid;
}
Program::State Program::get_state()
{
    return state;
}

void Program::start()
{
    update_state(STARTING);

    char *const env[]={NULL};

    char *const *arg=argv.get();

    State s=UNKNOWN;
    pid=fork();
    if(pid==0)
    {
        execve(arg[0],arg,env);
        printf("Game over ..........\n");
        exit(1);
    }
    else if (pid>0)
    {
        sleep(1);
        int ret=waitpid(pid,NULL,WNOHANG);
        printf("i am here..........\n");
        if (pid==ret)
            s=FATAL;
        else if (pid==0)
            s=RUNNING;
        else
            s=UNKNOWN;
        update_state(s);
    }
    else
    {
        update_state(s);
    }
}

void Program::stop()
{
    update_state(STOPPING);
    if (kill(pid,9)<0)
    {
        update_state(UNKNOWN);
    }
    else
    {
        update_state(STOPPED);
    }
}


