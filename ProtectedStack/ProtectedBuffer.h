#ifndef PROTECTED_BUFFER_H_INCLUDED
#define PROTECTED_BUFFER_H_INCLUDED

#include "Memory.h"

const uint8_t g_pbuff_poison_byte = (uint8_t)(((off_t)("Poison")) >> (sizeof (off_t)*4));

enum ProtectedBufferState 
{
    PBUFF_OK,
    PBUFF_BAD_DATA_FRONT_CANARY = 1024, PBUFF_BAD_DATA_BACK_CANARY,
    PBUFF_BAD_MEMORY,
    PBUFF_BAD_ELEM_SIZE,
};

struct ProtectedBuffer
{
    Memory mem;
    size_t elem_sz;
};

int protected_buff_allocate   (ProtectedBuffer *this_, size_t n_elems, size_t el_sz);
int protected_buff_deallocate (ProtectedBuffer *this_);
int protected_buff_reallocate (ProtectedBuffer *this_ ,size_t n_elems);
size_t protected_buff_size    (const ProtectedBuffer *this_);

void *protected_buff_get_data (const ProtectedBuffer *this_, size_t el_pos);
int protected_buff_set_data (ProtectedBuffer *this_, size_t el_pos, const void *new_data);
bool protected_buff_verify (const ProtectedBuffer *this_);
ProtectedBufferState protected_buffer_state (const ProtectedBuffer *this_);

#endif