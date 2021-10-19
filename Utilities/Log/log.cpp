#include "log.h"

#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>

static const size_t msg_sz = 1024;
static       size_t msg_off = 0;  // last formated_msg offset, not localized msg
static       char msg_buff[msg_sz + 1] = {};

#define MSG_END       (msg_buff + msg_off)
#define MSG_AVAILABLE (msg_sz - msg_off)

LOG_WIHTOUT_TRACE
char *get_formated_msg (MSG_TYPE type, const char *format_line, va_list args_start);

//void logger_message (MSG_TYPE type, const char *format_line, ...)
//{
//    Logger *instance = logger_get_instance();
//    FILE *file = instance->file;
//    int indent = (int)instance->indent;
//
//    auto formated_msg = "";
//    if (format_line)
//    {
//        va_list arg_list;    
//        va_start (arg_list, format_line);    
//        auto formated_msg = get_formated_msg (type, format_line, arg_list);   
//        va_end (arg_list);
//    }
//
//    if (format_line)
//    {
//        if (!logger_get_instance()->dumping)
//        {
//            save_fprintf (file, "%*.s", 4*indent, ""); // printing 4*indent spaces        
//            save_fprintf (file, formated_msg);
//            save_fprintf (file, "\n");
//            save_fflush (file);
//            if (type == FATAL)
//            {
//                // temporary desision, may be an call of user-fatal-error-function
//                exit (1);
//            }
//        }
//        else // dumping mode has no formating
//        {
//            push_va_fprintf (file, formated_msg);    
//            save_fflush (file);                    
//        }
//    }
//}

char *logger_get_formated_msg_localized (Location loc, MSG_TYPE type, const char *format_line, ...)
{
    va_list arg_list;    
    va_start (arg_list, format_line);    
    auto formated_msg = get_formated_msg (type, format_line, arg_list);   
    va_end (arg_list);

    snprintf (MSG_END, MSG_AVAILABLE, " <%s:%s:%d>", loc.file_name, loc.function_name, loc.nline);

    return formated_msg;
}

char *logger_get_formated_msg (MSG_TYPE type, const char *format_line, ...)
{
    va_list arg_list;    
    va_start (arg_list, format_line);    
    auto formated_msg = get_formated_msg (type, format_line, arg_list);   
    va_end (arg_list);

    return formated_msg;
}

char *get_formated_msg (MSG_TYPE type, const char *format_line, va_list args_start)
{
    msg_off = 0;

    auto type_s = str_type (type);     
    msg_off += snprintf (MSG_END, MSG_AVAILABLE, "[%s] ", type_s);

    if (LOG <= type && type <= FATAL)
    {
        msg_off += snprintf (MSG_END, MSG_AVAILABLE, "- \"");
        msg_off += vsnprintf (MSG_END, MSG_AVAILABLE, format_line, args_start);
        msg_off += snprintf (MSG_END, MSG_AVAILABLE, "\"");
    }
    else
    {
        msg_off += vsnprintf (MSG_END, MSG_AVAILABLE, format_line, args_start);
    }

    return msg_buff;
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