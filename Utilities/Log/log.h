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

LOG_WIHTOUT_TRACE
Logger *logger_get_instance();

LOG_WIHTOUT_TRACE
void logger_set_log_file_path (const char *path);

LOG_WIHTOUT_TRACE
void logger_message (MSG_TYPE type, const char *format_line, ...);

LOG_WIHTOUT_TRACE
void logger_message_dateiled (MSG_TYPE type, const char *file, 
    const char *func, size_t line, const char *format_line, ...);
#define LOG_MSG( type, ... ) \
logger_message_dateiled (type, __FILE__, __func__, __LINE__, __VA_ARGS__);

LOG_WIHTOUT_TRACE
void logger_indent_dec();

LOG_WIHTOUT_TRACE
void logger_indent_inc();

LOG_WIHTOUT_TRACE
void logger_start_dumping (const char *title);

LOG_WIHTOUT_TRACE
void logger_raw_str (const char *format_line, ...);

LOG_WIHTOUT_TRACE
void logger_raw_str_with_indent (const char *format_line, ...);

LOG_WIHTOUT_TRACE
void logger_finish_dumping();

LOG_WIHTOUT_TRACE
void logger_print_indent();

#endif