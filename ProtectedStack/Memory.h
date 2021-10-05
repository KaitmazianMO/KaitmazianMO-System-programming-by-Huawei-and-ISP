#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED
 
#include <stdlib.h>
#include <stdio.h>

#include "Canary.h"

enum MEMORY_ERRORS
{
    MEM_SUCCESS = 0, 
    MEM_ALLOCATION_FAILED = 256, MEM_REALLOCATION_FAILED,
    MEM_APPEAL_OUT_OF_RANGE, MEM_INVALID_ARGUMENTS,
};

typedef void (*error_handler_t) (MEMORY_ERRORS);

struct Memory
{
    char   *data;
    size_t  capacity; 
};

int mem_allocate   (Memory *this_, size_t n_elems, size_t el_size);
int mem_deallocate (Memory *this_);
int mem_reallocate (Memory *this_ ,size_t n_elems, size_t el_size);
int mem_fill (Memory *this_, char byte, size_t from, size_t to);

void *mem_get_data (const Memory *this_, size_t el_offset, size_t el_sz);
int mem_set_data (Memory *this_, size_t el_offset, size_t el_sz, const void *new_data, size_t data_sz);
bool mem_verify (const Memory *this_);

#endif