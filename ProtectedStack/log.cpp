#include "log.h"

#include <assert.h>
#include <stdarg.h>

static const char *log_file_name = ".log";

Logger *logger_get_instance()
{
    static Logger instance = {
        .log_file_name = log_file_name,
        .file = fopen (log_file_name, "wb"),
        .indent = 0,
        .dumping = false
    };
    assert (instance.file && "Can't opent a log file!");
    return &instance; 
}

void logger_set_log_file_path (const char *path)
{
    log_file_name = path;
}

void logger_message (MSG_TYPE type, const char *format_line, ...)
{
    if (logger_get_instance()->dumping) return;
    Logger *instance = logger_get_instance();
    FILE *file = instance->file;
    int indent = (int)instance->indent;

    if (format_line && instance)
    {
        auto type_s = str_type (type);
        fprintf (file, "%*.s", 4*indent, ""); // printing 4*indent spaces
        fprintf (file, "[%s] ", type_s);
        if (type != CALL && type != QUIT && type != DUMP) fprintf (file, "- ");
        va_list arg_list_p;
        va_start (arg_list_p, format_line);
        vfprintf (file, format_line, arg_list_p);
        va_end (arg_list_p);
        fputc ('\n', file);
        fflush (file);
    }
}

void logger_message_dateiled (MSG_TYPE type, const char *file_s, 
    const char *func_s, size_t line, const char *format_line, ...)
{
    if (logger_get_instance()->dumping) return;    
    Logger *instance = logger_get_instance();
    FILE *file = instance->file;
    int indent = (int)instance->indent;
   
    if (format_line && instance)
    {
        // logger_message (type, format_line, __ARGS__);
        // fseek (get_instance()->file, SEEK_CURR, -1);
        // fprintf (file, " <%s:%s:%zu>", file_s, func_s, line);
        auto type_s = str_type (type);
        fprintf (file, "%*.s", 4*indent, ""); // printing 4*indent spaces
        fprintf (file, "[%s] ", type_s);
        if (type != CALL && type != QUIT && type != DUMP) fprintf (file, "- ");
        va_list arg_list_p;
        va_start (arg_list_p, format_line);
        vfprintf (file, format_line, arg_list_p);
        va_end (arg_list_p);
        fprintf (file, " <%s:%s:%zu>", file_s, func_s, line);
        fputc ('\n', file);
        fflush (file);
    }
}

void logger_start_dumping (const char *title)
{
    logger_message (DUMP, title);
    logger_get_instance()->dumping = true;
}

void logger_raw_str (const char *format_line, ...)
{
    assert (logger_get_instance()->dumping);

    Logger *instance = logger_get_instance();
    FILE *file = instance->file;
    int indent = (int)instance->indent;
    
    if (format_line && instance)
    {
        va_list arg_list_p;
        va_start (arg_list_p, format_line);
        vfprintf (file, format_line, arg_list_p);
        va_end (arg_list_p);
        fflush (file);
    }
}

void logger_print_indent()
{
    fprintf (logger_get_instance()->file, "%*.s", (int)(4*logger_get_instance()->indent), "");
}

void logger_finish_dumping()
{
    logger_get_instance()->dumping = false;
}

void logger_indent_dec()
{
    ++logger_get_instance()->indent;
}

void logger_indent_inc()
{
    --logger_get_instance()->indent;
}

const char *str_type (MSG_TYPE type) 
{
    static char call[]    = "CALL";
    static char quit[]    = "QUIT";        
    static char fatal[]   = "FATAL";
    static char error[]   = "ERROR";
    static char warning[] = "WARING";
    static char log[]     = "LOG";
    static char dump[]    = "DUMP";
    
    if (type == CALL)    return call;
    if (type == QUIT)    return quit;    
    if (type == FATAL)   return fatal;
    if (type == ERROR)   return error;
    if (type == WARNING) return warning;
    if (type == DUMP)    return dump;
    return log;
}