#ifndef BUFFER_H_INCLUDED
#define BUFFER_H_INCLUDED
 
#include <stdlib.h>
#include <stdio.h>

enum BUFFER_ERRORS
{
    BUFF_SUCCESS = 0, 
    BUFF_ALLOCATION_FAILED = 256, BUFF_REALLOCATION_FAILED,
    BUFF_APPEAL_OUT_OF_RANGE
};

typedef void (*error_handler_t) (BUFFER_ERRORS);

struct Buffer
{
    char    *data;
    size_t   capacity;
    size_t  elem_size;
};

BUFFER_ERRORS buf_ctor         (Buffer *_this, size_t cap, size_t el_sz);
BUFFER_ERRORS buf_ctor_by_file (Buffer *_this, FILE *pfile);
BUFFER_ERRORS buf_resize       (Buffer *_this, size_t nelems);    
BUFFER_ERRORS buf_offset       (Buffer *_this, char **paddr, size_t idx);
BUFFER_ERRORS buf_dtor         (Buffer *_this);

void  buf_set_error_handler    (error_handler_t error_handler);
void  buf_ctor_handled         (Buffer *_this, size_t cap, size_t el_sz);
void  buf_ctor_by_file_handled (Buffer *_this, FILE *pfile);
char *buf_offset_handled       (Buffer *_this, size_t idx);
void  buf_resize_handled       (Buffer *_this, size_t nelems);    
void  buf_dtor_handled         (Buffer *_this);

#endif