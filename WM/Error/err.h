#ifndef ERR_H_INCLUDED
#define ERR_H_INCLUDED

#include <stdio.h>

size_t get_nerrors();
size_t get_nwarnings();

void set_err_stream (FILE *stream);
FILE *get_err_stream();

#define FATAL( fmt, ... )\
do {    \
    fatal (fmt, __VA_ARGS__);   \
    fprintf (get_err_stream(), "from: <%s:%s:%d>\n", __FILE__, __FUNCTION__, __LINE__);  \
} while (0)

#define ERROR( fmt, ... )\
do {    \
    error (fmt, __VA_ARGS__);   \
    fprintf (get_err_stream(), "from: <%s:%s:%d>\n", __FILE__, __FUNCTION__, __LINE__);  \
} while (0)

#define WARNING( fmt, ... )\
do {    \
    warning (fmt, __VA_ARGS__);   \
    fprintf (get_err_stream(), "\from: <%s:%s:%d>\n", __FILE__, __FUNCTION__, __LINE__);  \
} while (0)


void fatal   (const char *fmt, ...);
void error   (const char *fmt,  ...);
void warning (const char *fmt, ...);

#endif // ERR_H_INCLUDED