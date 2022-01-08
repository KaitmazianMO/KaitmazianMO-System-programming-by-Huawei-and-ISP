#include "log.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

static void do_raw_log (LoggerLocation loc, LOG_MSG_TYPE type, const char *msg, va_list *pargs);
static void do_html_log (LoggerLocation loc, LOG_MSG_TYPE type, const char *msg, va_list *pargs);
static void print_raw_type_prefix (LOG_MSG_TYPE type);
static void print_raw_msg_body (LoggerLocation loc, const char *msg, va_list *pargs);
static void print_html_type_prefix (LOG_MSG_TYPE type);
static void print_html_msg_body (LoggerLocation loc, const char *msg, va_list *pargs);
static const char *html_type_str (LOG_MSG_TYPE type);
static const char *raw_type_str (LOG_MSG_TYPE type);

static LoggerContext CONTEXT = {
    .lvl       = LOG_MSG_INFO,
    .file_name = NULL,
    .format    = LOG_FORMAT_RAW
};

void log_set_context_format (LOG_FORMAT fmt) {
    CONTEXT.format = fmt;
}

void log_set_context_lvl (LOG_MSG_TYPE log_lvl) {
    CONTEXT.lvl = log_lvl;
}

void log_set_context_file_name (const char *file_name) {
    CONTEXT.file_name = file_name;
}

FILE *new_output_stream() {
    auto stream = CONTEXT.file_name ? fopen (CONTEXT.file_name, "wb") : stderr;
    if (!stream) {
        stream = stderr;
        fprintf (stream, "Switched to stderr output mode\n");
        fprintf (stream, "Error when opening file %s.\n", CONTEXT.file_name);
    }

    if (setvbuf (stream, NULL, _IONBF, 0)) {
        fprintf (stream, "Error when disabling buffering\n");
    }

    if (CONTEXT.format == LOG_FORMAT_HTML) {
        fprintf (stream, "<pre>\n");
    }

    return stream;
}

Logger *log_instance() {
    static Logger instance {
        .context = CONTEXT,
        .output  = new_output_stream()
    }; assert (instance.output && "Log stream is NULL");

    return &instance;
}

void log_msg (LoggerLocation loc, LOG_MSG_TYPE type, const char *msg, ...) {
    if (log_instance()->context.lvl <= type) {
        va_list args = {};
        va_start (args, msg);
    
        log_msg_varg (loc, type, msg, &args);

        va_end (args);
    }
}

void log_msg_varg (LoggerLocation loc, LOG_MSG_TYPE type, const char *msg, va_list *pargs) {
    const auto format = log_instance()->context.format;
    switch (format) {
        case LOG_FORMAT_RAW:  do_raw_log  (loc, type, msg, pargs); break;
        case LOG_FORMAT_HTML: do_html_log (loc, type, msg, pargs); break;
        default:              do_raw_log  (loc, type, msg, pargs); break;
    }
    
    if (type == LOG_MSG_FATAL) {
        exit (EXIT_FAILURE);
    }
}

void do_raw_log (LoggerLocation loc, LOG_MSG_TYPE type, const char *msg, va_list *pargs) {
    print_raw_type_prefix (type);
    fprintf (log_instance()->output, " - ");
    print_raw_msg_body (loc, msg, pargs);
}

void do_html_log (LoggerLocation loc, LOG_MSG_TYPE type, const char *msg, va_list *pargs) {
    print_html_type_prefix (type);
    fprintf (log_instance()->output, " - ");
    print_html_msg_body (loc, msg, pargs);
}

void print_raw_type_prefix (LOG_MSG_TYPE type) {
    fprintf (log_instance()->output, "[%s]", raw_type_str (type));
}

void print_raw_msg_body (LoggerLocation loc, const char *msg, va_list *pargs) {
    auto output = log_instance()->output; 
    vfprintf (output, msg, *pargs);
    fprintf (output, " (%s:%s:%d)\n", loc.file, loc.func, loc.nline);        
}

void print_html_type_prefix (LOG_MSG_TYPE type) {
    fprintf (log_instance()->output, "[%s]", html_type_str (type));
}

void print_html_msg_body (LoggerLocation loc, const char *msg, va_list *pargs) {
    auto output = log_instance()->output; 
    vfprintf (output, msg, *pargs);
    fprintf (output, " <font color=\"grey\">(%s:%s:%d)</font>\n", loc.file, loc.func, loc.nline);        
}


static const char *html_type_str (LOG_MSG_TYPE type) {
    switch (type) {
        case LOG_MSG_INFO:    return "<font color=\"blue\">INFO   </font>";
        case LOG_MSG_WARNING: return "<font color=\"#F6BE00\">WARNING</font>";
        case LOG_MSG_ERROR:   return "<font color=\"red\">ERROR  </font>";
        case LOG_MSG_FATAL:   return "<font color=\"purple\">FATAL  </font>";
    }

    return "<font color=\"black\">UNKONW_TYPE   </font>";
}

static const char *raw_type_str (LOG_MSG_TYPE type) {
    switch (type) {
        case LOG_MSG_INFO:    return "INFO   ";
        case LOG_MSG_WARNING: return "WARNING";
        case LOG_MSG_ERROR:   return "ERROR  ";
        case LOG_MSG_FATAL:   return "FATAL  ";
    }   

    return "UNKNOWN_TYPE"; 
}