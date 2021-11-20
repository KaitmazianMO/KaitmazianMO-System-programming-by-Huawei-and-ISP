#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

#include <cstdio>
#include <cstdlib>

#define AKINATOR_FATAL( ... )   \
do {    \
    std::fprintf (stderr, __VA_ARGS__);    \
    std::fprintf (stderr, " (%s:%s:%d)\n", __FILE__, __func__, __LINE__ - 3);    \
    exit (1);   \
} while (0)

#endif // ERROR_H_INCLUDED