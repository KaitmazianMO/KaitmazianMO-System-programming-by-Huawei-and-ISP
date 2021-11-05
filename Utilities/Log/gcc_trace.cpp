#include "gcc_trace.h"
#include "../c_dangeon.h"
#include "log.h"

#include <stdio.h>
#include <dlfcn.h>
#include <cxxabi.h>

/*
 *  @todo: learn BFD (Binary File Descriptor library). Helps to do more datailed logs
 */

#ifdef $DO_TRACE
    #define TRACE_CODE( ... ) __VA_ARGS__
#else
    #define TRACE_CODE( ... ) ;
#endif

#ifndef $TRACE_FILE_NAME 
    #define $TRACE_FILE_NAME ".trace"
#endif

TRACE_CODE
(
    struct TraceContoller 
    {
        FILE  *file;
        int    indent;
        bool   do_trace;
    };

    WITHOUT_TRACE
    TraceContoller *tracer()
    {
        static TraceContoller trace_controller {
            .file       = fopen ($TRACE_FILE_NAME , "wb"),
            .indent     = 0,
            .do_trace   = true,
        };
        assert (trace_controller.file && "TRACE FILE OPEN FAILED");
        UNUSED (trace_controller);
        return &trace_controller;
    }
)

struct CallInfo
{
    const char *func_name;
    const char *file_name;
    bool        func_demangled;
};

WITHOUT_TRACE
CallInfo get_info (void *callee, void *caller);

WITHOUT_TRACE
void trace (MSG_TYPE type, CallInfo info);

void __cyg_profile_func_enter (void *callee, void *caller) 
{
    UNUSED (callee);
    UNUSED (caller);
    TRACE_CODE(    
        if (tracer()->do_trace)
        {
            CallInfo info = get_info (callee, caller);
            trace (CALL, info);
            if (info.func_demangled)
                delete info.func_name;
            ++tracer()->indent;
        }
    )
}

void __cyg_profile_func_exit (void *callee, void *caller) 
{
    UNUSED (callee);
    UNUSED (caller);
    TRACE_CODE (
        if (tracer()->do_trace)
        {
            --tracer()->indent;
            CallInfo info = get_info (callee, caller);
            trace (QUIT, info);
            if (info.func_demangled)
                delete info.func_name;
        }
    )
}

WITHOUT_TRACE
CallInfo get_info (void *callee, void *caller)
{
    UNUSED (caller);
    UNUSED (callee);
    CallInfo call_info = { "unknown", "unknown", false };
    Dl_info info = {};
    if (dladdr (callee, &info)) 
    {
        int status = 0;
        char* demangled = abi::__cxa_demangle (info.dli_sname, nullptr, 0, &status);
        if (status == 0) 
        {
            call_info.func_name = demangled ? demangled : "[not demangled]";
        } 
        else 
        {
            call_info.func_name = info.dli_sname ? info.dli_sname : "[no dli_sname]";
        }
        if (demangled) 
        {
            call_info.func_demangled = true;
        }
        call_info.file_name = info.dli_fname;
    } 
    return call_info;
}

#define save_fprintf( file, ... ) file && fprintf (file, __VA_ARGS__)
#define save_fflush( file )       file && fflush (file);

WITHOUT_TRACE
void trace (MSG_TYPE type, CallInfo info)
{
    UNUSED (type);
    UNUSED (info);
TRACE_CODE (
    auto formated_msg = logger_get_formated_msg (type, "%s <%s>", info.func_name, info.file_name);
    save_fprintf (tracer()->file, "%*.s", 4*tracer()->indent, "");
    save_fprintf (tracer()->file, "%s\n", formated_msg);
    save_fflush (tracer()->file);
)
}

void gcc_trace_msg (const char *msg)
{
    UNUSED (msg);
TRACE_CODE (
    save_fprintf (tracer()->file, "%*.s", 4*tracer()->indent, "");
    save_fprintf (tracer()->file, "%s\n", msg);
    save_fflush (tracer()->file);
)
}

void gcc_trace_stop_tracing()
{
TRACE_CODE (    
    tracer()->do_trace = false;
)
}

void gcc_trace_start_tracing()
{
TRACE_CODE (    
    tracer()->do_trace = true;
)
}