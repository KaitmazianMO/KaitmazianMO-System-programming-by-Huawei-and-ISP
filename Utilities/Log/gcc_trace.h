#ifndef GCC_TRACE_INCLUDED
#define GCC_TRACE_INCLUDED

#include <dlfcn.h>
#include <cxxabi.h>

#define WITHOUT_TRACE  __attribute__((no_instrument_function))

extern "C" WITHOUT_TRACE
void __cyg_profile_func_enter (void *callee, void *caller);
 
extern "C" WITHOUT_TRACE
void __cyg_profile_func_exit (void *callee, void *caller);

WITHOUT_TRACE
void gcc_trace_msg (const char *msg);

#endif