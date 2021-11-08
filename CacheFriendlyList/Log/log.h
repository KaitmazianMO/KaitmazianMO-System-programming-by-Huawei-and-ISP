#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#include <stdio.h>

enum LogType {
    INFO, 
    WARNING,
    ERROR,
    FATAL
};

#ifdef DO_LOGS     
    #define LOG_MSG( type , ...)    \
        do {    \
            const char *msg = log_format_msg (type,     \
                Location{.file = __FILE__, .func = __FUNCTION__, .nline = __LINE__}, __VA_ARGS__);   \
            log_msg (msg);  \
        } while (0)

    #define LOG_IMAGE( img_path )   \
        do {    \
            log_msg ("<img src=\"%s\">", img_path); \
        } while (0)

#else 
    #define LOG_MSG( type, ... )   ; 
    #define LOG_IMAGE( img_path )   ;
#endif 

struct Location {
    const char *file;
    const char *func;
    int nline;
};

void log_init (const char *log_file_name, LogType log_lvl = INFO);
LogType log_set_log_lvl (LogType lvl);
void log_msg (const char *msg, ...);
char *log_format_msg (LogType type, Location loc, const char *fmt, ...);

#endif //LOG_H_INCLUDED