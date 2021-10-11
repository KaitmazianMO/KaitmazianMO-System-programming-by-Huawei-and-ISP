#include "Stack.h"
#include "ProtectedBuffer.h"
#include "hash.h"
#include "Canary.h"
#include <assert.h>
#include <string.h>

const size_t GROW_COEFF = 2;

#ifdef $CANARIES_PROTECTION
    #define CANARIES_PROTECTION_CODE( ... ) __VA_ARGS__
#else
    #define CANARIES_PROTECTION_CODE( ... )  ;
#endif

#define SET_CANARIES( this_ )   \
CANARIES_PROTECTION_CODE (  \
    set_canary (&this_->front_canary);  \
    set_canary (&this_->back_canary);  \
)

#ifdef $HASH_PROTECTION
    #define HASH_PROTECTION_CODE( ... ) __VA_ARGS__
#else
    #define HASH_PROTECTION_CODE( ... ) ;
#endif

#ifdef $MEMORY_CLEAN_UPS
    #define MEMORY_CLEAN_UPS_CODE( ... ) __VA_ARGS__
#else
    #define MEMORY_CLEAN_UPS_CODE( ... ) ;
#endif

#define UPDATE_HASHES( this_ )   \
HASH_PROTECTION_CODE (   \
    update_hashes (this_)   \
)

struct Stack
{
CANARIES_PROTECTION_CODE (canary_t front_canary; )
    ProtectedBuffer     buff;
    size_t              curr_pos;
    stk_elem_printer_t  print_elem;
HASH_PROTECTION_CODE (    
    hash_t              data_hash;
    hash_t              stack_hash;
)
CANARIES_PROTECTION_CODE (canary_t back_canary; )
};

HASH_PROTECTION_CODE (
    hash_t compute_stack_hash (const Stack *this_);
    hash_t compute_buff_hash (const Stack *this_);
    void update_hashes (Stack *this_);
    bool check_stack_hash (const Stack *this_); 
    bool check_data_hash (const Stack *this_);
)

MEMORY_CLEAN_UPS_CODE (
    void clean_elem (Stack *this_, size_t n);
)

#define CLEAN_ELEM( this_, n) MEMORY_CLEAN_UPS_CODE (clean_elem (this_, n); )

Stack *stack_ctor (size_t size, size_t elem_sz)
{
    Stack *pstk = (Stack *)calloc (1, sizeof (*pstk));
    if (pstk)
    { 
        int err = protected_buff_allocate (&pstk->buff, size, elem_sz);
        if (err)
        {
            free (pstk);
            return NULL;
        }
        pstk->curr_pos = 0;    
        pstk->print_elem = NULL;
        SET_CANARIES (pstk);
        UPDATE_HASHES (pstk);
    }

    return pstk;
}

int stack_push (Stack *this_, const void *data)
{
    assert (stack_verify (this_));

    int err = 0;
    if (protected_buff_size (&this_->buff) == this_->curr_pos)
        err = protected_buff_reallocate (&this_->buff, (this_->curr_pos + (this_->curr_pos ? 0 : 1)) * GROW_COEFF);

    if (!err)
    {
        int err = protected_buff_set_data (&this_->buff, this_->curr_pos, data);
        if (!err) this_->curr_pos++;
    }
    UPDATE_HASHES (this_);
    return err;
}

int stack_pop (Stack *this_, void *dest)
{
    assert (stack_verify (this_));
    assert (dest);

    if (this_->curr_pos == 0) return STK_POPPING_EMPTY_STACK;

    auto ptop = protected_buff_get_data (&this_->buff, --this_->curr_pos);
    memmove (dest, ptop, this_->buff.elem_sz);
    CLEAN_ELEM (this_, this_->curr_pos);


    UPDATE_HASHES (this_);    
    return STACK_OK;
}

int stack_dtor (Stack *this_)
{
    assert (stack_verify (this_));

    int err= 0;
    err = protected_buff_deallocate (&this_->buff);
    memset (this_, g_pbuff_poison_byte, sizeof (*this_));
    free (this_);   

    return err;   
}

WITHOUT_TRACE
static void print_indent (FILE *file, int indent) { fprintf (file, "%*.s", indent, ""); }

void stack_dump (const Stack *this_, FILE *file, int indent)
{
    assert (stack_verify (this_));
    if (file == NULL) file = stderr;

    START_DUMPING ("Stack dump");

    CANARIES_PROTECTION_CODE (
        print_indent (file, indent); fprintf (file, "\tfront canary     = %lx\n", this_->front_canary);
    )
    print_indent (file, indent); fprintf (file,     "\tcurrent top      = %zu\n", this_->curr_pos);
    print_indent (file, indent); fprintf (file,     "\tcurrent capacity = %zu\n", protected_buff_size (&this_->buff));
    HASH_PROTECTION_CODE (
        print_indent (file, indent); fprintf (file, "\tstack hash       = " HASH_FMT "\n", this_->stack_hash);
        print_indent (file, indent); fprintf (file, "\tdata hash        = " HASH_FMT "\n", this_->data_hash);
    )
    CANARIES_PROTECTION_CODE (
        print_indent (file, indent); fprintf (file, "\tback canary      = %lx\n", this_->back_canary);
    )

    const void *curr_elem_ptr = NULL;
    print_indent (file, indent);
    fprintf (file, "\t{\n");
    const size_t buff_size = protected_buff_size (&this_->buff);
    for (size_t i = 0; i < buff_size; ++i)
    {
        curr_elem_ptr = protected_buff_get_data (&this_->buff, i);
        print_indent (file, indent);
        fprintf (file, "\t\t[%zu] = ", i);
        if (this_->print_elem) 
            this_->print_elem (file, curr_elem_ptr);
        else 
            fprintf (file, "[not match operator to print an element]");
        fprintf (file, "\n");
    }
    print_indent (file, indent);
    fprintf (file, "\t}\n");

    FINISH_DUMPING();
}

void stack_dump_to_log (const Stack *this_)
{
    assert (stack_verify (this_));

    START_DUMPING ("Stack dump");

    stack_dump (this_, LOG_INSTANCE()->file, 4*LOG_INSTANCE()->indent);

    FINISH_DUMPING();
}

void stack_set_elem_printer (Stack *this_, stk_elem_printer_t printer)
{   
    assert (stack_verify (this_));
    assert (printer);

    this_->print_elem = printer;
    UPDATE_HASHES (this_);    
}

int stack_state (const Stack *this_)
{
    int state = STACK_OK;
    CANARIES_PROTECTION_CODE (
        if (!is_valid_canary (&(this_->front_canary))) 
            state |= STK_BAD_FRONT_CANARY;
        if (!is_valid_canary (&this_->back_canary))  
            state |= STK_BAD_BACK_CANARY;
    )
    if (!protected_buff_verify (&this_->buff))   
        state |= STK_BUFFER_VERIFICATION_FAILED;
    HASH_PROTECTION_CODE (
        if (!check_stack_hash (this_)) 
            state |= STK_HASH_MISMATCH;
        if (!check_data_hash (this_))
            state |= STK_DATA_HASH_MISMATCH;;    
    )
    return state;   
}

bool stack_verify (const Stack *this_)
{
    assert (this_);

    int state = stack_state (this_);
    CANARIES_PROTECTION_CODE (
    if (state & STK_BAD_FRONT_CANARY)
        LOG_MSG_LOC (ERROR, "Bad front canary %llx(%p)", this_->front_canary, &this_->front_canary);
    if (state & STK_BAD_BACK_CANARY)  
        LOG_MSG_LOC (ERROR, "Bad back canary %llx(%p)", this_->back_canary, &this_->back_canary);
    )
    
    if (state & STK_BUFFER_VERIFICATION_FAILED)   
        LOG_MSG_LOC (ERROR, "Buffer verifying failed");
    
    HASH_PROTECTION_CODE (
        if (state & STK_HASH_MISMATCH) 
            LOG_MSG_LOC (ERROR, "Stack hash was suddenly changed from outside,");
        if (state & STK_DATA_HASH_MISMATCH)
            LOG_MSG_LOC (ERROR, "Stack data hash was suddenly changed from outside");    
    )
    return state == STACK_OK;
}

HASH_PROTECTION_CODE (
    void update_hashes (Stack *this_) 
    {   
        this_->data_hash   = 0;
        this_->stack_hash = 0;

        this_->data_hash  = compute_buff_hash (this_);
        this_->stack_hash = compute_stack_hash (this_); 
    }

    hash_t compute_stack_hash (const Stack *this_) 
    { 
        return compute_hash ((const char *)this_, sizeof (*this_)); 
    }

    hash_t compute_buff_hash (const Stack *this_)  
    { 
        return compute_hash ((const char *)protected_buff_get_data (&this_->buff, 0), 
            protected_buff_size (&this_->buff)*this_->buff.elem_sz); 
    }

    bool check_stack_hash (const Stack *this_)
    {
        Stack *this_non_const = (Stack *)this_;
        hash_t old_hash = this_->stack_hash;
        this_non_const->stack_hash = 0;
        hash_t new_hash = compute_stack_hash (this_);
        this_non_const->stack_hash = old_hash;

        return old_hash == new_hash;
    }

    bool check_data_hash (const Stack *this_)
    {
        Stack *this_non_const = (Stack *)this_;    
        hash_t old_hash = this_non_const->data_hash;
        this_non_const->data_hash = 0;
        hash_t new_hash = compute_buff_hash (this_);
        this_non_const->data_hash = old_hash;

        return old_hash == new_hash;
    }
)

MEMORY_CLEAN_UPS_CODE (
    void clean_elem (Stack *this_, size_t n)
    {
        assert (this_);
        
        auto pelem = protected_buff_get_data (&this_->buff, n);
        memset (pelem, g_pbuff_poison_byte, this_->buff.elem_sz);
    }
)