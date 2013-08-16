#include "Daemon.hpp"

Daemon::Daemon()
{
}

Daemon::~Daemon()
{
    for (ProgramMap::iterator it=v_program.begin();it!=v_program.end();++it)
    {
        delete it->second;
        it->second=NULL;
    }
}

Program *Daemon::get_program(const char *name)
{
    //FIXME: what if not found??
    Program *p=v_program[name];
    
    return p;
}

void Daemon::stop(const char *name)
{
    Program *p=get_program(name);
    if (p!=NULL) p->stop();
}

void Daemon::start(const char *name)
{
    Program *p=get_program(name);
    if (p!=NULL) p->start();
}

void Daemon::add(const char *name,const char *cmd)
{
    v_program[name]=new Program(cmd);
}

Program::State Daemon::update_state_if_terminated(Program *p)
{
    Program::State origin_state=p->get_state();
    Program::State current_state=Program::UNKNOWN;
    switch (origin_state)
    {
        case Program::STARTING:
            current_state=Program::FATAL;
            break;
        case Program::STOPPING:
            current_state=Program::STOPPED;
            break;
        case Program::RUNNING:
            current_state=Program::EXITED;
            break;
        default:
            break;
    }
    p->update_state(current_state);
    return origin_state; 
}

void Daemon::loop()
{
    printf("waitpid .......\n");
    while (true)
    {
        int pid=waitpid(-1,NULL,0);
        if (pid<0)
        {
            printf("DONE  \n");
            return;
        }

        for (ProgramMap::iterator it=v_program.begin();it!=v_program.end();++it)
        {
            Program *p=(it->second);
            if (pid==p->get_pid())
            {
                Program::State s=update_state_if_terminated(p);
                switch (s)
                {
                case Program::RUNNING:
                case Program::STARTING:
                    p->start();
                    break;
                default:
                    break;
                }
            }
        }
    }
}


