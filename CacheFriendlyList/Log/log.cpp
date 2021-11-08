#include "log.h"
#include <assert.h>
#include <stdarg.h>

struct Logger {
    FILE *stream;
    LogType log_lvl;
};

static Logger logger = {};

static const char *str (LogType);

void log_init (const char *log_file_name, LogType log_lvl) {
    logger = { .stream = fopen (log_file_name, "w"), .log_lvl = log_lvl };
}

inline Logger *save_logger() {
    if (logger.stream == NULL) {
        logger.stream = stderr;
    }
    return &logger;
}

void log_msg (const char *msg, ...) {
    assert (msg);
    if (msg[0] && msg) {
        va_list arg = {};
        va_start (arg, msg);
        vfprintf (save_logger()->stream, msg, arg);
        va_end (arg);
        fprintf (save_logger()->stream, "\n");
        fflush (save_logger()->stream);
    }
}

char *log_format_msg (LogType type, Location loc, const char *fmt, ...) {
    const unsigned int buff_sz = 1024u;
    static char buff[buff_sz + 1]= {};

    if (logger.log_lvl >= type) {
        unsigned int off = 0;
        off += snprintf (buff, buff_sz - off, "[%7s] ", str (type));

        va_list start = {};
        va_start(start, fmt);
        off += vsnprintf (buff, buff_sz - off, fmt, start);
        va_end (start);

        off += snprintf (buff, buff_sz - off, "<%s:%s:%d>", loc.file, loc.func, loc.nline);        
    } else {
        buff[0] = '\0';
    }

    return buff;
}

static const char *str (LogType type) {
    switch (type)
    {
        case INFO:    return "INFO";
        case WARNING: return "WARNING";
        case ERROR:   return "ERROR";
        case FATAL:   return "FATAL";
    }
    return "INFO";
}

LogType log_set_log_lvl (LogType lvl) {
    auto old = save_logger()->log_lvl;
    save_logger()->log_lvl = lvl;
    return old;
}