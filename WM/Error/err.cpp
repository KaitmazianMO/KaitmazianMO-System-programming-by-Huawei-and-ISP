#include "err.h"
#include <stdarg.h>
 
static FILE *err_stream = stderr;
static size_t nerrors = 0u;
static size_t nwarnings = 0u;

void   count_error();
size_t get_nerrors();
void   count_warning();
size_t get_nwarnings();


void set_err_stream (FILE *stream) {
    if (stream) {
        err_stream = stream;
    }
}

FILE *get_err_stream() {
    return err_stream ? err_stream : stderr;
}

void fatal (const char *fmt, ...) {
    fprintf (get_err_stream(), "FATAL: ");
    va_list list;
    va_start (list, fmt);
    vfprintf (get_err_stream(), fmt, list);
    va_end (list);
    fprintf (get_err_stream(), "\n");
}

void error (const char *fmt, ...) {
    count_error();
    va_list list;
    va_start (list, fmt);
    vfprintf (get_err_stream(), fmt, list);
    va_end (list);
    fprintf (get_err_stream(), "\n");
}

void warning (const char *fmt, ...) {
    count_warning();
    fprintf (get_err_stream(), "WARNING: ");
    va_list list;    
    va_start (list, fmt);
    vfprintf (get_err_stream(), fmt, list);
    va_end (list);
    fprintf (get_err_stream(), "\n");
}

void   count_error() { ++nerrors; }
size_t get_nerrors() { return nerrors; }
void   count_warning() { ++nwarnings; }
size_t get_nwarnings() { return nwarnings; }
