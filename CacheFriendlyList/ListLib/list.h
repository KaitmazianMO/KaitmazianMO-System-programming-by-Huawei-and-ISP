#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <stddef.h>

typedef double val_t;
typedef size_t ref_t;

class List {
public: 
    struct Node {
       val_t val;
       ref_t next;
       ref_t prev;
    };

    static constexpr ref_t DEFAULT_CAPACITY = 32u;
    static constexpr ref_t BAD_REF = (ref_t)-1;   
    
public:
    explicit List (ref_t cap = List::DEFAULT_CAPACITY);   
   ~List();
    ref_t head() const;
    ref_t tail() const;
    ref_t next (ref_t ref) const;
    ref_t prev (ref_t ref) const;
    
    ref_t insert_front (val_t val);
    ref_t insert_back (val_t val);
    ref_t insert_after (ref_t ref, val_t val);
    ref_t insert_before (ref_t ref, val_t val);
    ref_t erase (ref_t ref);
    
    val_t &get (ref_t ref);
    const val_t &get (ref_t ref) const;
    bool is_valid_ref (ref_t ref) const;


private:
    Node *nodes;
    ref_t head_ref;
    ref_t tail_ref;
    ref_t free_head_ref;
    ref_t size;
    ref_t capacity;

    ref_t free_head() const;
    void  shift_free_head();
    ref_t allocate_val (val_t val);
};

int   list_init (List *this_, ref_t cap = List::DEFAULT_CAPACITY);
int   list_free (List *this_);
val_t list_get (List *this_, ref_t ref);

ref_t list_head (List *this_);
ref_t list_tail (List *this_);
ref_t list_next (List *this_, ref_t ref);
ref_t list_prev (List *this_, ref_t ref);
ref_t list_insert_front (List *this_, val_t val);
ref_t list_insert_back (List *this_, val_t val);
ref_t list_insert_after (List *this_, ref_t ref, val_t val);
ref_t list_insert_before (List *this_, ref_t ref, val_t val);
ref_t list_erase (List *this_, ref_t ref);
bool  is_valid_ref (List *this_, ref_t ref);

#endif