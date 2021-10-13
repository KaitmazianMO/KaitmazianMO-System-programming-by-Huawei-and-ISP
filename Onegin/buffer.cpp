#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "buffer.h"
#include "../Utilities/Log/gcc_trace.h"
#include "../Utilities/stdlib_addition.h"

void buff_default_error_handler (BUFFER_ERRORS);
static error_handler_t err_handler = buff_default_error_handler;

#define RETURN_IF_TRUE( condition, ERR_CODE ) if (condition) { return ERR_CODE; }

BUFFER_ERRORS buf_ctor (Buffer *_this, size_t cap, size_t el_sz)
{
    assert (_this);

    _this->data = (char *)calloc (cap, el_sz);
    RETURN_IF_TRUE (_this->data == NULL, BUFF_ALLOCATION_FAILED)

    _this->capacity  = cap;
    _this->elem_size = el_sz;

    return BUFF_SUCCESS;
}

BUFFER_ERRORS buf_ctor_by_file (Buffer *_this, FILE *pfile)
{
    assert (_this);
    assert (pfile);

    _this->elem_size = sizeof (char);
    _this->data = (char *)read_buff (pfile, &_this->capacity);
    if (_this->data == NULL)
    {
        return BUFF_ALLOCATION_FAILED;
    }

    return BUFF_SUCCESS;
}

BUFFER_ERRORS buf_offset (Buffer *_this, char **paddr, size_t idx)
{
    assert (_this);
    assert (paddr);

    *paddr = NULL;
    RETURN_IF_TRUE (_this->capacity <= idx, BUFF_APPEAL_OUT_OF_RANGE);

    *paddr = _this->data + idx * _this->elem_size;
    return BUFF_SUCCESS;
}

BUFFER_ERRORS buf_resize (Buffer *_this, size_t nelems)
{
    assert (_this);
    assert (_this->data);

    char *new_addr = (char *)realloc (_this->data, _this->elem_size * nelems);
    RETURN_IF_TRUE (new_addr == NULL, BUFF_REALLOCATION_FAILED);

    _this->data = new_addr;
    _this->capacity = nelems;
    return BUFF_SUCCESS;
}

BUFFER_ERRORS buf_dtor (Buffer *_this)
{
    assert (_this);

    free (_this->data);
    _this->data      = NULL;
    _this->capacity  = 0;
    _this->elem_size = 0;
    return BUFF_SUCCESS;
}

#undef RETURN_IF_TRUE
#define BUFF_HANDLED_PATTERN_CALL( buf_func, ... )         \
{ BUFFER_ERRORS err =  buf_func (__VA_ARGS__);             \
      if (err != BUFF_SUCCESS) { err_handler (err); } }    \

void buf_ctor_handled (Buffer *_this, size_t cap, size_t el_sz)
{
    BUFF_HANDLED_PATTERN_CALL (buf_ctor, _this, cap, el_sz)
}
void buf_ctor_by_file_handled (Buffer *_this, FILE *pfile)
{
    BUFF_HANDLED_PATTERN_CALL (buf_ctor_by_file, _this, pfile)    
}
char *buf_offset_handled (Buffer *_this, size_t idx)
{
    char *addr = NULL;
    BUFF_HANDLED_PATTERN_CALL (buf_offset, _this, &addr, idx)
    return addr;
}
void buf_resize_handled (Buffer *_this, size_t nelems)
{
    BUFF_HANDLED_PATTERN_CALL (buf_resize, _this, nelems);
}
void buf_dtor_handled (Buffer *_this)
{
    BUFF_HANDLED_PATTERN_CALL (buf_dtor, _this);
}
#undef BUFF_HANDLED_PATTERN_CALL

void buf_set_error_handler (error_handler_t error_handler)
{
    assert (err_handler);
    err_handler = error_handler; 
}

void buff_default_error_handler (BUFFER_ERRORS err)
{
    fprintf (stderr, "Error: %d\n", err);

    exit (err);
}