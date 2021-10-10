#include "gcc_trace.h"

#include <stdio.h>
#include <bfd.h>
#include <dlfcn.h>
#include <cxxabi.h>

/*
 *  @todo: learn BFD (Binary File Descriptor library). Helps to do more datailed logs
 */

WITHOUT_TRACE
void damangle (const char *name, const char file);

struct CallInfo
{
    const char *func_name;
    const char *file_name;
};

void __cyg_profile_func_enter (void *callee, void *caller) 
{
    Dl_info info;
    if (dladdr (callee, &info)) 
    {
        int status = 0;
        const char* name = NULL;
        char* demangled = abi::__cxa_demangle (info.dli_sname, nullptr, 0, &status);
        if (status == 0) 
        {
            name = demangled ? demangled : "[not demangled]";
        } 
        else 
        {
            name = info.dli_sname ? info.dli_sname : "[no dli_sname]";
        }
        LOG_MSG (CALL, "%s <%s>", name, info.dli_fname);
        if (demangled) 
        {
            delete demangled;
            demangled = nullptr;
        }
    } 

    logger_indent_dec();
}

void __cyg_profile_func_exit (void *callee, void *caller) 
{
    logger_indent_inc();
    Dl_info info = {};
    if (dladdr (callee, &info)) 
    {
        int status = 0;
        const char* name = NULL;
        char* demangled = abi::__cxa_demangle (info.dli_sname, nullptr, 0, &status);
        if (status == 0) 
        {
            name = demangled ? demangled : "[not demangled]";
        } 
        else 
        {
            name = info.dli_sname ? info.dli_sname : "[no dli_sname]";
        }
        LOG_MSG (QUIT, "%s <%s>", name, info.dli_fname); //fprintf(log, "%*s[QUIT] %s <%s>\n", 4*--stk_offset, "", name, info.dli_fname);
        if (demangled) 
        {
            delete demangled;
            demangled = nullptr;
        }
    }
    else
    {

    }
}