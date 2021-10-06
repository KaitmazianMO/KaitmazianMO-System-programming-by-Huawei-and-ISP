#include "Stack.h"
#include "../Utilities/Log/gcc_trace.h"
#include "../Utilities/Log/log.h"

#include <stdio.h>
#include <assert.h>

void print_int (FILE *file, const void *pint);

int main()
{
    int data = 5;
    auto *stk_int = stack_ctor (10, sizeof (int));
    if (!stk_int) LOG_MSG_LOC (FATAL, "Stack was not created");

    stack_dump_to_log (stk_int);

    for (int i = 0; i < 128; ++i)
        stack_push (stk_int, &i), printf ("%d\n", i);

    for (int i = 0; stack_pop (stk_int, &data) == STACK_OK; ++i)
    {
        printf ("[%d] = %d\n", i, data);
    }

    stack_set_elem_printer (stk_int, print_int);

    stack_dump_to_log (stk_int);
    stack_dtor (stk_int);    

    return 0;
}

void print_int (FILE *file, const void *pint)
{
    assert (pint);

    int val = *(int *)(pint);
    fprintf (file, "%d", val);
}