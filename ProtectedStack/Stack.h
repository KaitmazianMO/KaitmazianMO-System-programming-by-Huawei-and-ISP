#ifndef STACK_H_IONCLUDED
#define STACK_H_IONCLUDED

#include <stdlib.h>
#include "../Utilities/Log/gcc_trace.h"
#include "ProtectedBuffer.h"


enum StackState
{
    STACK_OK                        = 0x00,
    STK_BAD_FRONT_CANARY            = 0x01,     
    STK_BAD_BACK_CANARY             = 0x02,
    STK_HASH_MISMATCH               = 0x04, 
    STK_DATA_HASH_MISMATCH          = 0x08,
    STK_POPPING_EMPTY_STACK         = 0x10,
    STK_BUFFER_VERIFICATION_FAILED  = 0x20
};

typedef void (*stk_elem_printer_t) (FILE *file, const void *elem);
typedef void (*stk_error_handler_t) (int err);

struct Stack;

Stack *stack_ctor (size_t size, size_t elem_sz);
int    stack_push (Stack *this_, const void *data);
int    stack_pop (Stack *this_, void *dest);
int    stack_dtor (Stack *this_);
bool   stack_verify (const Stack *this_);
int    stack_state (const Stack *this_);
void   stack_set_elem_printer (Stack *this_, stk_elem_printer_t printer);
void   stack_dump (const Stack *this_, FILE *file, int indent);
void   stack_dump_to_log (const Stack *this_);

#endif