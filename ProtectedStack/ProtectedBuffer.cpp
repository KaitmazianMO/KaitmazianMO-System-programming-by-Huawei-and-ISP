#include "ProtectedBuffer.h"
#include "Canary.h"
#include "../Utilities/Log/log.h"
#include <assert.h>
#include <memory.h>

#ifndef $PROTECTION_OFF
#define ON_PROTECTION_MODE( ... ) __VA_ARGS__
#else 
#define ON_PROTECTION_MODE
#endif

canary_t *get_front_canary (const ProtectedBuffer *this_);
canary_t *get_back_canary (const ProtectedBuffer *this_);
void fill_poison_all_after (ProtectedBuffer *buff, size_t n_el);

int protected_buff_allocate   (ProtectedBuffer *this_, size_t n_elems, size_t el_sz)
{
    assert (this_);

    auto err = mem_allocate (&this_->mem, n_elems*el_sz + 2*sizeof(canary_t), 1);
    if (err) return err;
    
    fill_poison_all_after (this_, 0);
    this_->elem_sz = el_sz;
    set_canary (get_front_canary (this_));
    set_canary (get_back_canary (this_));

    return MEM_SUCCESS;
}

int protected_buff_deallocate (ProtectedBuffer *this_)
{
    assert (protected_buff_verify (this_));

    fill_poison_all_after (this_, 0);
    this_->elem_sz = 0;
    return mem_deallocate (&this_->mem);
}

int protected_buff_reallocate (ProtectedBuffer *this_ ,size_t n_elems)
{
    assert (protected_buff_verify (this_));

    const size_t old_n_elems = protected_buff_size (this_);   
    int err = mem_reallocate (&this_->mem, n_elems*this_->elem_sz + 2*sizeof(canary_t), 1);
    if (!err)
    {
        if (old_n_elems < n_elems)
            fill_poison_all_after (this_, old_n_elems);
        set_canary (get_front_canary (this_));
        set_canary (get_back_canary (this_));
    }
    return err;
}

void *protected_buff_get_data (const ProtectedBuffer *this_, size_t el_pos)
{
    assert (protected_buff_verify (this_));

    return mem_get_data (&this_->mem, el_pos*this_->elem_sz + sizeof (canary_t), 1);
}

int protected_buff_set_data (ProtectedBuffer *this_, size_t el_pos, const void *new_data)
{
    assert (protected_buff_verify (this_));

    return mem_set_data (&this_->mem, el_pos*this_->elem_sz + sizeof(canary_t), 1, new_data, this_->elem_sz);
}

size_t protected_buff_size    (const ProtectedBuffer *this_)
{
    assert (protected_buff_verify (this_));
    return (this_->mem.capacity - 2*sizeof (canary_t))/this_->elem_sz;
}

ProtectedBufferState protected_buffer_state (const ProtectedBuffer *this_)
{
    assert (this_);

    auto pfront_canary = get_front_canary (this_);
    auto pback_canary = get_back_canary (this_);
    if (!mem_verify (&this_->mem)       ) return PBUFF_BAD_MEMORY;
    if (!this_->elem_sz                 ) return PBUFF_BAD_ELEM_SIZE;
    if (!is_valid_canary (pfront_canary)) return PBUFF_BAD_DATA_FRONT_CANARY;
    if (!is_valid_canary (pback_canary) ) return PBUFF_BAD_DATA_BACK_CANARY;
    
    return PBUFF_OK;
}

bool protected_buff_verify (const ProtectedBuffer *this_)
{
    assert (this_);

    auto state = protected_buffer_state (this_);
    {
        if (state == PBUFF_BAD_MEMORY)            LOG_MSG_LOC (ERROR, "Mem verifying failed");
        if (state == PBUFF_BAD_ELEM_SIZE)         LOG_MSG_LOC (ERROR, "Elem size is zero"); 
        if (state == PBUFF_BAD_DATA_FRONT_CANARY) LOG_MSG_LOC (ERROR, "Bad front canary %llx(%p)", *get_front_canary (this_), get_front_canary (this_));
        if (state == PBUFF_BAD_DATA_BACK_CANARY)  LOG_MSG_LOC (ERROR, "Bad back canary %llx(%p)", *get_back_canary (this_), get_back_canary (this_));   
    }

    return state == PBUFF_OK;
}

canary_t *get_front_canary (const ProtectedBuffer *this_) 
{ 
    return (canary_t *)this_->mem.data; 
}

canary_t *get_back_canary (const ProtectedBuffer *this_)  
{
    return (canary_t *)(this_->mem.data + this_->mem.capacity - sizeof(canary_t)); 
}
void fill_poison_all_after (ProtectedBuffer *buff, size_t n_el) 
{ 
    mem_fill (&buff->mem, g_pbuff_poison_byte, n_el*buff->elem_sz + sizeof (canary_t), 
                                               buff->mem.capacity - sizeof (canary_t)); 
}
