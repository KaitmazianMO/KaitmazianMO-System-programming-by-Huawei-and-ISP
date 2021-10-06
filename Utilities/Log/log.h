#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#include <stdio.h>

#define LOG_WIHTOUT_TRACE  __attribute__((no_instrument_function))

enum MSG_TYPE : char
{
    CALL = 'C',
    QUIT = 'Q',     
    DUMP = 'D',

     FATAL   = 'F',
     ERROR   = 'E',
    WARNING  = 'W',
      LOG    = 'L'
};

LOG_WIHTOUT_TRACE
const char *str_type (MSG_TYPE type);

struct Logger 
{
    const char *log_file_name;
    FILE       *file ;
    size_t      indent;
    bool        dumping;  // mode for writing multy-line messages almost without formating
};

#ifndef $LOG_OFF
    #define LOG_MSG_LOC( type, ... ) \
    logger_message_localized (type, __FILE__, __func__, __LINE__, __VA_ARGS__)

    #define LOG_INSTANCE() (logger_get_instance())

    #define LOG_SET_FILE_NAME( name ) logger_set_log_file_path (name)

    #define LOG_MSG( type, ... )  logger_message (type, __VA_ARGS__)

    #define LOG_INC()  logger_indent_inc()

    #define LOG_DEC()  logger_indent_dec()

    #define START_DUMPING( title )  logger_start_dumping (title)

    #define LOG_RAW_STR( ... )  logger_raw_str (__VA_ARGS__)

    #define FINISH_DUMPING()  logger_finish_dumping()

    #define LOG_PRINT_INDENT()  logger_print_indent()
#else
    #define LOG_MSG_LOC( type, ... ) ;

    #define LOG_INSTANCE() ;

    #define LOG_SET_FILE_NAME( name )  ;

    #define LOG_MSG( type, ... )  ;

    #define LOG_INC()  ;

    #define LOG_DEC()  ;

    #define START_DUMPING( title )  ;

    #define LOG_RAW_STR( ... )  ;

    #define FINISH_DUMPING() ;

    #define LOG_PRINT_INDENT()  ;
#endif
    

LOG_WIHTOUT_TRACE
Logger *logger_get_instance();

LOG_WIHTOUT_TRACE
void logger_set_log_file_path (const char *path);

LOG_WIHTOUT_TRACE
void logger_message (MSG_TYPE type, const char *format_line, ...);

LOG_WIHTOUT_TRACE
void logger_message_localized (MSG_TYPE type, const char *file, 
    const char *func, size_t line, const char *format_line, ...);

LOG_WIHTOUT_TRACE
void logger_indent_dec();

LOG_WIHTOUT_TRACE
void logger_indent_inc();

LOG_WIHTOUT_TRACE
void logger_start_dumping (const char *title);

LOG_WIHTOUT_TRACE
void logger_raw_str (const char *format_line, ...);

LOG_WIHTOUT_TRACE
void logger_finish_dumping();

LOG_WIHTOUT_TRACE
void logger_print_indent();

#endif