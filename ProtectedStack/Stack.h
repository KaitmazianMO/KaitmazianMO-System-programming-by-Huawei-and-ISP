#ifndef STACK_H_IONCLUDED
#define STACK_H_IONCLUDED

#include <stdlib.h>
#include "../Utilities/Log/gcc_trace.h"
#include "ProtectedBuffer.h"

enum StackState
{
    STACK_OK,
    STK_BAD_FRONT_CANARY,     STK_BAD_BACK_CANARY,
    STK_BAD_DATA_FRONT_CANARY = PBUFF_BAD_DATA_FRONT_CANARY,
    STK_BAD_DATA_BACK_CANARY = PBUFF_BAD_DATA_BACK_CANARY,
    STK_HASH_MISMATCH, STK_DATA_HASH_MISMATCH,
    STK_POPPING_EMPTY_STACK
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
void   stack_dump_to_log (const Stack *this_);

#endif