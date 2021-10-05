#ifndef TOKEN_H_INCLUDED
#define TOKEN_H_INCLUDED

#include <ctype.h>

struct Token
{
    char *beg;
    size_t size;
};

#endif