#include "Stack.h"
#include "ProtectedBuffer.h"
#include "hash.h"
#include "Canary.h"
#include <assert.h>
#include <string.h>

const size_t GROW_COEFF = 2;

#ifdef CANARY_PROTECTION
    #define SET_CANARIES
        
    #define VERIFY_CANAIES
#endif



struct Stack
{
    canary_t front_canary;
    ProtectedBuffer     buff;
    size_t              curr_pos;
    hash_t              data_hash;
    hash_t              stack_hash;
    stk_elem_printer_t  print_elem;
    canary_t back_canary;
};

hash_t compute_stack_hash (const Stack *this_);
hash_t compute_buff_hash (const Stack *this_);
void update_hashes (Stack *this_);
bool check_stack_hash (const Stack *this_); 
bool check_data_hash (const Stack *this_);

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
        set_canary (&pstk->front_canary);
        set_canary (&pstk->back_canary);
        pstk->curr_pos = 0;    
        pstk->print_elem = NULL;
        update_hashes (pstk);
    }

    return pstk;
}

int stack_push (Stack *this_, const void *data)
{
    assert (stack_verify (this_));

    int err = 0;
    if (protected_buff_size (&this_->buff) == this_->curr_pos)
        err = protected_buff_reallocate (&this_->buff, this_->curr_pos*GROW_COEFF + 1);

    if (!err)
    {
        int err = protected_buff_set_data (&this_->buff, this_->curr_pos, data);
        if (!err) this_->curr_pos++;
    }
    update_hashes (this_);
    return err;
}

int stack_pop (Stack *this_, void *dest)
{
    assert (stack_verify (this_));
    assert (dest);

    if (this_->curr_pos == 0) return STK_POPPING_EMPTY_STACK;

    auto ptop = protected_buff_get_data (&this_->buff, --this_->curr_pos);
    memmove (dest, ptop, this_->buff.elem_sz);

    update_hashes (this_);    
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

int stack_state (const Stack *this_)
{
    if (!is_valid_canary (&(this_->front_canary))) 
        return STK_BAD_FRONT_CANARY;
    if (!is_valid_canary (&this_->back_canary))  
        return STK_BAD_BACK_CANARY;
    if (!protected_buff_verify (&this_->buff))   
        return protected_buffer_state (&this_->buff);
    if (!check_stack_hash (this_)) 
        return STK_HASH_MISMATCH;
    if (!check_data_hash (this_))
        return STK_DATA_HASH_MISMATCH;;    

    return STACK_OK;   
}

void stack_dump_to_log (const Stack *this_)
{
    assert (stack_verify (this_));

    logger_start_dumping ("Stack dump");

    logger_print_indent(); logger_raw_str ("\tfront canary     = %llx\n", this_->front_canary);
    logger_print_indent(); logger_raw_str ("\tcurrent top      = %zu\n", this_->curr_pos);
    logger_print_indent(); logger_raw_str ("\tcurrent capacity = %zu\n", protected_buff_size (&this_->buff));
    logger_print_indent(); logger_raw_str ("\tstack hash       = " HASH_FMT "\n", this_->stack_hash);
    logger_print_indent(); logger_raw_str ("\tdata hash        = " HASH_FMT "\n", this_->data_hash);
    logger_print_indent(); logger_raw_str ("\tcurrent top      = %zu\n", this_->curr_pos);
    logger_print_indent(); logger_raw_str ("\tback canary      = %llx\n", this_->back_canary);

    const void *curr_elem_ptr = NULL;
    logger_print_indent();
    logger_raw_str ("\t{\n");
    for (size_t i = 0; i < protected_buff_size (&this_->buff); ++i)
    {
        curr_elem_ptr = protected_buff_get_data (&this_->buff, i);
        logger_print_indent();
        logger_raw_str ("\t\t[%d] = ", i);
        if (this_->print_elem) this_->print_elem (logger_get_instance()->file, curr_elem_ptr);
        else logger_raw_str ("[not match operator to print an element]");
        logger_raw_str ("\n");
    }
    logger_print_indent();
    logger_raw_str ("\t}\n");

    logger_finish_dumping();
}

void stack_set_elem_printer (Stack *this_, stk_elem_printer_t printer)
{   
    assert (stack_verify (this_));
    assert (printer);

    this_->print_elem = printer;
    update_hashes (this_);    
}

bool stack_verify (const Stack *this_)
{
    assert (this_);

    int state = stack_state (this_);
    if (state == STK_BAD_FRONT_CANARY)
        LOG_MSG_LOC (ERROR, "Bad front canary %llx(%p)", this_->front_canary, &this_->front_canary);
    if (state == STK_BAD_BACK_CANARY)  
        LOG_MSG_LOC (ERROR, "Bad back canary %llx(%p)", this_->back_canary, &this_->back_canary);
    if (state == STK_BAD_DATA_BACK_CANARY || state == STK_BAD_DATA_FRONT_CANARY)   
        LOG_MSG_LOC (ERROR, "Buffer verifying failed");
    if (state == STK_HASH_MISMATCH) 
        LOG_MSG_LOC (ERROR, "Stack hash was suddenly changed from outside,");
    if (state == STK_DATA_HASH_MISMATCH)
        LOG_MSG_LOC (ERROR, "Stack data hash was suddenly changed from outside");    

    return state == STACK_OK;
}

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