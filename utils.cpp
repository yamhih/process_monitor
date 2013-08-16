#include "utils.hpp"

int supper_strchr(const char **s,char c)
{
    const char *tmp=*s;
    while(*tmp==c)
    {
        ++tmp;
    }
    *s=tmp;
    const char *tmp2=strchr(tmp,c);
    if(tmp2==NULL)
    {
        return strlen(tmp);
    }
    else
    {
        return tmp2-tmp;
    }
}


