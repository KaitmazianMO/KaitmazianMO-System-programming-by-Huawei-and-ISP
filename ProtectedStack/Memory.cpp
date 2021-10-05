#include "Memory.h"
#include "log.h"
#include <assert.h>
#include <string.h>

error_handler_t err_handler = NULL;

#define RETURN_IF_TRUE( cond, error )           \
    if (cond)                                   \
    {                                           \
        return error;                           \
    }                                           \

int mem_allocate   (Memory *this_, size_t n_elems, size_t el_size)
{
    assert (this_);
    assert (el_size);

    this_->data = (char *)calloc (n_elems, el_size);
    RETURN_IF_TRUE (this_->data == NULL, MEM_ALLOCATION_FAILED);

    this_->capacity = n_elems * el_size;

    LOG_MSG (LOG, "Allocated %zu bytes at %p", this_->capacity,  this_->data);
    return MEM_SUCCESS;
}

int mem_deallocate (Memory *this_)
{
    assert (mem_verify (this_));

    free (this_->data);
    this_->data = NULL;
    this_->capacity = 0; 
    return MEM_SUCCESS;
}

int mem_reallocate (Memory *this_ ,size_t n_elems, size_t el_size)
{
    assert (mem_verify (this_));
    assert (el_size);

    const size_t new_cap = n_elems*el_size;
    char *new_loc = (char *)realloc (this_->data, new_cap);
    RETURN_IF_TRUE (new_loc == NULL, MEM_REALLOCATION_FAILED);

    this_->data = new_loc;
    this_->capacity = new_cap;
    return MEM_SUCCESS;
}

void *mem_get_data (const Memory *this_, size_t el_offset, size_t el_sz)
{
    assert (mem_verify (this_));
    assert (el_sz);

    return (void *)(this_->data + el_offset*el_sz);
}

int mem_set_data (Memory *this_, size_t el_offset, size_t el_sz, const void *new_data, size_t data_sz)
{
    assert (mem_verify (this_));
    assert (el_sz);
    assert (new_data);
    assert (data_sz);

    auto dest = mem_get_data (this_, el_offset, el_sz);
    memmove (dest, new_data, data_sz);
    return MEM_SUCCESS;
}

int mem_fill (Memory *this_, char byte, size_t from, size_t to)
{
    assert (mem_verify (this_));
    if (to <= from)
        return MEM_INVALID_ARGUMENTS;
    if (this_->capacity < to)
        return MEM_APPEAL_OUT_OF_RANGE;
    
    memset (this_->data + from, byte, to - from);
    return MEM_SUCCESS; 
}

bool mem_verify (const Memory *this_)
{
    return this_ && this_->data && this_->capacity;
}
