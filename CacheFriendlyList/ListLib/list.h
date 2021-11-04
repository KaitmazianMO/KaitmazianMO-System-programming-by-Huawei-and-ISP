#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <stddef.h>
#include <bitset>

typedef double val_t;
typedef size_t ref_t;

constexpr ref_t LIST_POOL_SIZE = 1024u;
constexpr ref_t LIST_BAD_REF = (ref_t)-1;

struct List {
    ref_t head_ref;
    /* the keys are located in an array to reduce the number of cache misses when iterating over the list */
    val_t pool[LIST_POOL_SIZE];
    /* next[i] - next element after pool[i] 
       or if next[i] == i, pool[i] is free */
    ref_t next[LIST_POOL_SIZE];
    
    size_t size;
};

int   list_init (List *this_);
int   list_free (List *this_);

val_t list_get (List *this_, ref_t ref);

ref_t list_insert_front (List *this_, val_t val);
ref_t list_insert_after (List *this_, ref_t ref, val_t val);
ref_t list_erase (List *this_, ref_t ref);

#endif