#include "ProtectedBuffer.h"
#include "Canary.h"
#include "../Utilities/Log/log.h"
#include <assert.h>
#include <memory.h>

static protected_buff_error_handler_t err_handler = NULL;

#ifdef $CANARIES_PROTECTION
    #define CANARIES_PROTECION_CODE( ... )  __VA_ARGS__
    #define SET_CANARIES(this_)  set_canary (get_front_canary (this_)); \
                                 set_canary (get_back_canary (this_));   

    static const size_t CANARY = sizeof (canary_t);

#else 
    #define CANARIES_PROTECION_CODE( ... ) ;
    #define SET_CANARIES                   ;  
    static const size_t CANARY = 0;
#endif

#ifdef $MEMORY_CLEAN_UPS
    #define MEMORY_CLEAN_UPS_CODE( ... )  __VA_ARGS__
    #define FILL_WITH_POISON( this_ ) fill_poison_all_after (this_, 0)

#else 
    #define MEMORY_CLEAN_UPS_CODE( ... ) ;
    #define FILL_WITH_POISON( this_ )    ;    
#endif

#ifdef $STRUCT_VERIFICATION
    #define STRUCT_VERIDICATION_CODE( ... ) __VA_ARGS__

#else
    #define STRUCT_VERIDICATION_CODE( ... ) ;
#endif

#define VERIFY( this_ )     \
STRUCT_VERIDICATION_CODE    \
(   \
    {   \
        int state = PBUFF_OK;   \
        if ((state = protected_buff_state (this_)) != PBUFF_OK)     \
            return state;   \
    }   \
)

#define NULL_VERIFY( this_ )     \
STRUCT_VERIDICATION_CODE    \
(   \
    {   \
        int state = PBUFF_OK;   \
        if ((state = protected_buff_state (this_)) != PBUFF_OK)     \
            return NULL;   \
    }   \
)


CANARIES_PROTECION_CODE (
    canary_t *get_front_canary (const ProtectedBuffer *this_);
    canary_t *get_back_canary (const ProtectedBuffer *this_);    
)

MEMORY_CLEAN_UPS_CODE (
    void fill_poison_all_after (ProtectedBuffer *buff, size_t n_el);    
)

int protected_buff_allocate   (ProtectedBuffer *this_, size_t n_elems, size_t el_sz)
{
    assert (this_);

    auto err = mem_allocate (&this_->mem, n_elems*el_sz + 2*CANARY, 1);
    if (err) return err;
    
    this_->elem_sz = el_sz;
    FILL_WITH_POISON (this_);
    SET_CANARIES (this_);

    return MEM_SUCCESS;
}

int protected_buff_deallocate (ProtectedBuffer *this_)
{
    VERIFY (this_);

    FILL_WITH_POISON (this_);
    this_->elem_sz = 0;
    return mem_deallocate (&this_->mem);
}

int protected_buff_reallocate (ProtectedBuffer *this_ ,size_t n_elems)
{
    VERIFY (this_);


    MEMORY_CLEAN_UPS_CODE (
        const size_t old_n_elems = protected_buff_size (this_);  
    ) 
    int err = mem_reallocate (&this_->mem, n_elems*this_->elem_sz + 2*CANARY, 1);
    if (!err)
    {
        MEMORY_CLEAN_UPS_CODE (
            if (old_n_elems < n_elems) fill_poison_all_after (this_, old_n_elems);
        )
        SET_CANARIES (this_);
    }
    return err;
}

void *protected_buff_get_data (const ProtectedBuffer *this_, size_t el_pos)
{
    NULL_VERIFY (this_);
    return mem_get_data (&this_->mem, el_pos*this_->elem_sz + CANARY, 1);
}

int protected_buff_set_data (ProtectedBuffer *this_, size_t el_pos, const void *new_data)
{
    VERIFY (this_);
    return mem_set_data (&this_->mem, el_pos*this_->elem_sz + CANARY, 1, new_data, this_->elem_sz);
}

size_t protected_buff_size    (const ProtectedBuffer *this_)
{
    VERIFY (this_);
    return (this_->mem.capacity - 2*CANARY)/this_->elem_sz;
}

ProtectedBufferState protected_buff_state (const ProtectedBuffer *this_)
{
    assert (this_);
    
    int state = PBUFF_OK;
    if (!mem_verify (&this_->mem)       ) state |= PBUFF_BAD_MEMORY;
    if (!this_->elem_sz                 ) state |= PBUFF_BAD_ELEM_SIZE;

    CANARIES_PROTECION_CODE (
        auto pfront_canary = get_front_canary (this_);
        auto pback_canary = get_back_canary (this_);
        if (!is_valid_canary (pfront_canary)) state |= PBUFF_BAD_DATA_FRONT_CANARY;
        if (!is_valid_canary (pback_canary) ) state |= PBUFF_BAD_DATA_BACK_CANARY;
    )
    
    return (ProtectedBufferState)state;
}

bool protected_buff_verify (const ProtectedBuffer *this_)
{
    assert (this_);

    auto state = protected_buff_state (this_);
    {
        if (state & PBUFF_BAD_MEMORY)            
            LOG_MSG (ERROR, protected_buff_str_error (PBUFF_BAD_MEMORY));
        if (state & PBUFF_BAD_ELEM_SIZE)         
            LOG_MSG (ERROR, protected_buff_str_error (PBUFF_BAD_ELEM_SIZE)); 
        CANARIES_PROTECION_CODE (
            if (state & PBUFF_BAD_DATA_FRONT_CANARY) 
                LOG_MSG (ERROR, "%s %llx(%p)", protected_buff_str_error (PBUFF_BAD_DATA_FRONT_CANARY),
                                              *get_front_canary (this_), get_front_canary (this_));
            if (state & PBUFF_BAD_DATA_BACK_CANARY)  
                LOG_MSG (ERROR, "%s %llx(%p)", protected_buff_str_error (PBUFF_BAD_DATA_BACK_CANARY), 
                                              *get_back_canary (this_), get_back_canary (this_));   
        )
    }

    return state == PBUFF_OK;
}

const char *protected_buff_str_error (int err)
{
    if (err & PBUFF_BAD_MEMORY)            
        return "Mem verifying failed";
    if (err & PBUFF_BAD_ELEM_SIZE)         
        return "Elem size is zero"; 
    CANARIES_PROTECION_CODE (
        if (err & PBUFF_BAD_DATA_FRONT_CANARY) 
            return "Bad front canary";
        if (err & PBUFF_BAD_DATA_BACK_CANARY)  
            return "Bad back canary";
    )  
    return "";
}

protected_buff_error_handler_t protected_buff_set_error_handler (protected_buff_error_handler_t mew_handler)
{
    auto old_handler = err_handler;
    err_handler = mew_handler;
    return old_handler;
}

CANARIES_PROTECION_CODE 
(
    canary_t *get_front_canary (const ProtectedBuffer *this_) 
    { 
        return (canary_t *)this_->mem.data; 
    }

    canary_t *get_back_canary (const ProtectedBuffer *this_)  
    {
        return (canary_t *)(this_->mem.data + this_->mem.capacity - CANARY); 
    }
)

MEMORY_CLEAN_UPS_CODE (
    void fill_poison_all_after (ProtectedBuffer *buff, size_t n_el) 
    { 
        mem_fill (&buff->mem, g_pbuff_poison_byte, n_el*buff->elem_sz + CANARY, 
                                                   buff->mem.capacity - CANARY); 
    }
)