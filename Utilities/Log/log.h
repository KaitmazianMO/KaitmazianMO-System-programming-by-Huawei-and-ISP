#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#include <stdio.h>
#include <assert.h>

#include "../c_dangeon.h"

#ifdef $DO_LOG
    #define LOG_CODE( ... ) __VA_ARGS__
#else
    #define LOG_CODE( ... ) ;
#endif

#define LOG_WITHOUT_TRACE  __attribute__((no_instrument_function))

LOG_WITHOUT_TRACE
FILE *log_file();

#define LOG_LOC \
    Location{ .nline = __LINE__, .file_name = __FILE__, .function_name = __FUNCTION__ }

#ifdef $DO_TRACE
    #include "gcc_trace.h"
    #define TRACE( msg ) gcc_trace_msg (msg);
#else
    #define TRACE( msg ) ;
#endif

#ifdef $DO_LOG
    #include <stdlib.h> /* for exit */
    #define LOG_MSG( type, ... ) \
    do {    \
        const char *msg = logger_get_formated_msg_localized (LOG_LOC, type, __VA_ARGS__);    \
        fprintf (log_file(), "%s\n", msg);  \
        fflush (log_file());  \
        TRACE (msg);  \
        if (type == FATAL) exit (1);    \
    } while (0)

    #define LOG_STR( str )  \
    do {    \
        fprintf (log_file(), "%s\n", msg);    \
    } while (0) 
#else
    #define LOG_MSG( ... ) ;
    #define LOG_STR( str ) ;
#endif

#define DUMP_STR( str ) \
    do {    \
        LOG_STR (str);   \
        TRACE (str);    \
    } while (0)

#ifdef $DO_TRACE
    #define START_DUMPING( title )  \
    do {    \
        gcc_trace_stop_tracing();   \
        auto msg = logger_get_formated_msg (DUMP, title);   \
        LOG_STR (msg);   \
        TRACE (msg);    \
    } while (0)

    #define STOP_DUMPING    \
    do {    \
        gcc_trace_start_tracing();  \
    } while (0)    
#else
    #define START_DUMPING( title ) ;
    #define STOP_DUMPING  ;
#endif

enum MSG_TYPE : char
{
    CALL = 'C',
    QUIT = 'Q',     
    DUMP = 'D',

      LOG    = 'L',
    WARNING  = 'W',
     ERROR   = 'E',
     FATAL   = 'F',
};

struct Location 
{
    int         nline;
    const char *file_name;
    const char *function_name;
};

LOG_WITHOUT_TRACE
const char *str_type (MSG_TYPE type);

LOG_WITHOUT_TRACE
char *logger_get_formated_msg_localized (Location loc, MSG_TYPE type, const char *format_line, ...);

LOG_WITHOUT_TRACE
char *logger_get_formated_msg (MSG_TYPE type, const char *format_line, ...);

LOG_WITHOUT_TRACE
void logger_dump (const char *str);

#endif