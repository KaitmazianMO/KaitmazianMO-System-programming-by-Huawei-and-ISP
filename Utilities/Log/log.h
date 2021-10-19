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

#define LOG_WIHTOUT_TRACE  __attribute__((no_instrument_function))


#ifdef $DO_LOG
    FILE *log_file = stderr;
    #ifdef $LOG_FILE_NAME
        log_file = fopen ($LOG_FILE_NAME, "wb");
    #endif
#endif

#define strange_suffix_LOG_LOC \
    Location{ .nline = __LINE__, .file_name = __FILE__, .function_name = __FUNCTION__ }

#ifdef $DO_TRACE
    #define TRACE( msg ) gcc_trace_msg (msg);
#else
    #define TRACE( msg ) ;
#endif

#ifdef $DO_LOG
    #define LOG_MSG( type, ... ) \
    do {    \
        assert (log_file && "LOG FILE WAS NOT OPENED"); \
        auto msg = logger_get_formated_msg_localized (strange_suffix_LOG_LOC, type, __VA_ARGS__);    \
        fprintf (log_file, "%s\n", msg);  \
        fflush (log_file);  \
        TRACE (msg);  \
    } while (0)
#else
    #define LOG_MSG( ... ) ;
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

LOG_WIHTOUT_TRACE
const char *str_type (MSG_TYPE type);

LOG_WIHTOUT_TRACE
char *logger_get_formated_msg_localized (Location loc, MSG_TYPE type, const char *format_line, ...);

LOG_WIHTOUT_TRACE
char *logger_get_formated_msg (MSG_TYPE type, const char *format_line, ...);

#endif