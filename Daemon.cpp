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
    switch (origin_state)
    {
//    case Program::STARTING:
//        p->update_state(Program::FATAL);
//        break;
    case Program::STOPPING:
        p->update_state(Program::STOPPED);
        break;
    case Program::RUNNING:
        p->update_state(Program::EXITED);
        break;
    case Program::BACKOFF:
        p->update_state(Program::FATAL);
        break;
    default:
        p->update_state(Program::UNKNOWN);
        break;
    }
    return origin_state; 
}

void Daemon::show()
{
    printf("$$$$$$$$$$$$$$$$$$$$$$$\n");
    for (ProgramMap::iterator it=v_program.begin();it!=v_program.end();++it)
    {
        Program *p=(it->second);
        printf("%s:%d\n",it->first,p->get_state());
    }
    printf("---------------------\n");
}
void Daemon::loop()
{
    printf("waitpid .......\n");
    while (true)
    {
        show();
        int pid=waitpid(-1,NULL,0);
        if (pid<0)
        {
            perror("waitpid");
            printf("DONE  \n");
            sleep(10);
            continue;
        }

        for (ProgramMap::iterator it=v_program.begin();it!=v_program.end();++it)
        {
            Program *p=(it->second);
//            printf("%d==%s:%d\n",pid,it->first,p->get_state());
            if (pid==p->get_pid())
            {
                Program::State s=update_state_if_terminated(p);
                switch (s)
                {
                case Program::RUNNING:
                case Program::STARTING:
                case Program::BACKOFF:
                    p->start();
                    break;
                default:
                    break;
                }
            }
        }
    }
}


