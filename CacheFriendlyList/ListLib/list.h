#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <stddef.h>

typedef double val_t;
typedef size_t ref_t;

struct List {
   struct Node {
      val_t val;
      ref_t next;
      ref_t prev;
   };

   static constexpr ref_t DEFAULT_CAPACITY = 32u;
   static constexpr ref_t BAD_REF = (ref_t)-1;   
    
   Node *nodes;
   ref_t head_ref;
   ref_t tail_ref;
   ref_t free_head_ref;
    
   ref_t size;
   ref_t cap;
};

int   list_init (List *this_, ref_t cap = List::DEFAULT_CAPACITY);
int   list_free (List *this_);

val_t list_get (List *this_, ref_t ref);

bool  is_valid_ref (List *this_, ref_t ref);
ref_t list_insert_front (List *this_, val_t val);
ref_t list_insert_after (List *this_, ref_t ref, val_t val);
ref_t list_erase (List *this_, ref_t ref);

#endif