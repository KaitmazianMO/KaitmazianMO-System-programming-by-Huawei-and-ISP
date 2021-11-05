#ifndef STACK_H_IONCLUDED
#define STACK_H_IONCLUDED

#include <stdlib.h>
#include "ProtectedBuffer.h"


enum StackState
{
    STACK_OK                        = 0x00,
    STK_BAD_FRONT_CANARY            = 0x01,     
    STK_BAD_BACK_CANARY             = 0x02,
    STK_HASH_MISMATCH               = 0x04, 
    STK_DATA_HASH_MISMATCH          = 0x08,
    STK_POPPING_EMPTY_STACK         = 0x10,
    STK_PEEKING_EMPTY_STACK         = 0x20,
    STK_BUFFER_VERIFICATION_FAILED  = 0x40
};

struct Stack;

typedef void (*stk_elem_printer_t) (FILE *file, const void *elem);
typedef void (*stk_error_handler_t) (const Stack *this_, int state);


Stack *stack_ctor (size_t size, size_t elem_sz);
int    stack_push (Stack *this_, const void *data);
int    stack_pop (Stack *this_);
int    stack_top (const Stack *this_, void *dest);
int    stack_dtor (Stack *this_);

bool   stack_verify (const Stack *this_);
int    stack_state (const Stack *this_);
int    stack_set_elem_printer (Stack *this_, stk_elem_printer_t printer);
int    stack_dump (const Stack *this_, FILE *file, int indent);
int    stack_dump_to_log (const Stack *this_);

stk_error_handler_t stack_set_handler (stk_error_handler_t handler);
void stack_handle (const Stack *this_, int err);
const char *stack_str_state (int state);

inline void stack_push_handled (Stack *this_, const void *data)
{
    int state = stack_push (this_, data);
    if (state != STACK_OK)
        stack_handle (this_, state);
}

inline void   stack_pop_handled (Stack *this_)
{
    int state = stack_pop (this_);
    if (state != STACK_OK)
        stack_handle (this_, state);
}

inline void stack_top_handled (const Stack *this_, void *dest)
{
    int state = stack_top (this_, dest);
    if (state != STACK_OK)
        stack_handle (this_, state);
}

inline Stack *stack_ctor_handled (size_t size, size_t elem_sz)
{
    Stack *this_ = stack_ctor (size, elem_sz);
    if (this_)
        stack_handle (this_, -1u);
    
    return this_;
}

inline void stack_dtor_handled (Stack *this_)
{
    int state = stack_dtor (this_);
    if (state != STACK_OK)
        stack_handle (this_, state);
}

#endif