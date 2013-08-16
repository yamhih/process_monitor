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
    if (get_state()==RUNNING)
    {
        return pid;
    }
    return -1;
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

    int fds[2];
    if (pipe(fds)<0)
    {
        perror("pipe");
    }

    if (fcntl(fds[1],F_SETFD,1)<0)
    {
        perror("fcntl");
    }

    pid=fork();
    if(pid==0)
    {
        close(fds[0]);
        execve(arg[0],arg,env);
        printf("Game over ..........\n");
        write(fds[1],"w",1);
        close(fds[1]);
        exit(1);
    }
    else if (pid>0)
    {
        close(fds[1]);
        printf("i am here..........\n");

        char c;
        if (read(fds[0],&c,1)==0)
        {
            printf("execve success ...\n");
            update_state(RUNNING);
        }
        else
            update_state(BACKOFF);

        close(fds[0]);
    }
    else
    {
        update_state(UNKNOWN);
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


