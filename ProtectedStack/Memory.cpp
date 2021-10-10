#include "Memory.h"
#include "../Utilities/Log/log.h"
#include <assert.h>
#include <string.h>

static mem_error_handler_t err_handler = NULL;

#define RETURN_AND_HANDLE_IF_TRUE( cond, error, this_ )           \
    if (cond)                                                     \
    {                                                             \
        if (err_handler) err_handler (this_, error);              \
        return error;                                             \
    }                                                             \

#define RETURN_NULL_AND_HANDLE_IF_TRUE( cond, error, this_ )      \
    if (cond)                                                     \
    {                                                             \
        if (err_handler) err_handler (this_, error);              \
        return NULL;                                              \
    }       

int mem_allocate   (Memory *this_, size_t n_elems, size_t el_size)
{
    RETURN_AND_HANDLE_IF_TRUE (!this_ || !el_size, MEM_INVALID_ARGUMENTS, this_); 

    this_->data = (char *)calloc (n_elems, el_size);
    RETURN_AND_HANDLE_IF_TRUE (this_->data == NULL, MEM_ALLOCATION_FAILED, this_);

    this_->capacity = n_elems * el_size;

    LOG_MSG_LOC (LOG, "Allocated %zu bytes at %p", this_->capacity, this_->data);
    return MEM_SUCCESS;
}

int mem_deallocate (Memory *this_)
{
    RETURN_AND_HANDLE_IF_TRUE (!mem_verify (this_), MEM_VERIFY_FAILED, this_);

    free (this_->data);
    this_->data = NULL;
    this_->capacity = 0; 
    return MEM_SUCCESS;
}

int mem_reallocate (Memory *this_ ,size_t n_elems, size_t el_size)
{
    RETURN_AND_HANDLE_IF_TRUE (!mem_verify (this_), MEM_VERIFY_FAILED, this_);
    RETURN_AND_HANDLE_IF_TRUE (!el_size, MEM_INVALID_ARGUMENTS, this_);

    const size_t new_cap = n_elems*el_size;
    char *new_loc = (char *)realloc (this_->data, new_cap);
    RETURN_AND_HANDLE_IF_TRUE (new_loc == NULL, MEM_REALLOCATION_FAILED, this_);

    this_->data = new_loc;
    this_->capacity = new_cap;
    return MEM_SUCCESS;
}

void *mem_get_data (const Memory *this_, size_t el_offset, size_t el_sz)
{
    RETURN_NULL_AND_HANDLE_IF_TRUE (!mem_verify (this_), MEM_VERIFY_FAILED, this_);
    RETURN_NULL_AND_HANDLE_IF_TRUE (!el_sz, MEM_INVALID_ARGUMENTS, this_);

    return (void *)(this_->data + el_offset*el_sz);
}

int mem_set_data (Memory *this_, size_t el_offset, size_t el_sz, const void *new_data, size_t data_sz)
{
    RETURN_AND_HANDLE_IF_TRUE (!mem_verify (this_), MEM_VERIFY_FAILED, this_);
    RETURN_AND_HANDLE_IF_TRUE (!(el_sz && new_data && data_sz), MEM_INVALID_ARGUMENTS, this_);

    auto dest = mem_get_data (this_, el_offset, el_sz);
    memmove (dest, new_data, data_sz);
    return MEM_SUCCESS;
}

int mem_fill (Memory *this_, char byte, size_t from, size_t to)
{
    RETURN_AND_HANDLE_IF_TRUE (!mem_verify (this_), MEM_VERIFY_FAILED, this_);
    if (to <= from)
        return MEM_INVALID_ARGUMENTS;
    if (this_->capacity < to)
        return MEM_APPEAL_OUT_OF_RANGE;
    
    memset (this_->data + from, byte, to - from);
    return MEM_SUCCESS; 
}

mem_error_handler_t mem_set_error_handler (mem_error_handler_t new_handler)
{
    auto old_handler = err_handler;
    err_handler = new_handler;
    return old_handler; 
}

bool mem_verify (const Memory *this_)
{
    return this_ && this_->data && this_->capacity;
}
