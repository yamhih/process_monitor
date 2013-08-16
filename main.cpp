#include "Daemon.hpp"

int main()
{
    Daemon a;
    a.add("abc","./test -t 1000         oo");
    printf("starting abc......\n");
    a.start("abc");
    a.loop();

    return 0;
}
