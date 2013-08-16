#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char **argv)
{

    int time=210;
    if (argc>2)
    {
    }

    printf("I am running ........\n");
    sleep(time);
    printf("I am over ........\n");

    return 0;
}
