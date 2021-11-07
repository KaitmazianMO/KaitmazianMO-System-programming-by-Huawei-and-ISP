#include "list.h"
#include <assert.h>
#include <stdlib.h>

#define NEXT( list , ref )  (list->nodes[ref].next)
#define PREV( list , ref )  (list->nodes[ref].prev)
#define HEAD( list )        (list->head_ref)
#define TAIL( list )        (list->tail_ref)

ref_t list_allocate_val (List *list, val_t val) {
    auto ref = list->free_head_ref;
    if (ref != List::BAD_REF) {
        list->free_head_ref = NEXT (list, list->free_head_ref);
        list->nodes[ref].val = val;
        list->nodes[ref].next = List::BAD_REF;
        list->nodes[ref].prev = List::BAD_REF;
        ++list->size;
    }

    return ref;
}

void list_erase_ref (List *list, ref_t ref) {
    if (!is_valid_ref (list, ref)) { // already erased
        return;
    }

    PREV (list, ref) = ref; // mark as free;
    NEXT (list, ref) =  list->free_head_ref; // tie with free list
    list->free_head_ref = ref; 
    --list->size;
}

int list_init (List *list, ref_t cap) {
    assert (list);
    list->size = 0;
    list->cap = cap;
    list->nodes = (List::Node *)calloc (cap, sizeof (list->nodes[0]));
    if (!list->nodes) {
        return 0;
    }

    list->free_head_ref = 0;
    list->head_ref = list->free_head_ref;
    list->tail_ref = list->free_head_ref;
    for (int i = 0; i < list->cap; ++i) {
        list->nodes[i].prev = i;  // mark free vals
        list->nodes[i].next = i + 1;  // tie free list
    }
    list->nodes[list->cap - 1].next = List::BAD_REF;

    return 1;
}

int list_free (List *list) {
    assert (list);

    free (list->nodes);
    return 1;
}

ref_t list_insert_front (List *list, val_t val) {
    assert (list);

    auto new_front_ref = list_allocate_val (list, val);
    if (new_front_ref != List::BAD_REF) {
        NEXT (list, new_front_ref) = list->head_ref;
        if (HEAD (list) != List::BAD_REF) {
            PREV (list, HEAD (list)) = new_front_ref;
        }
        HEAD (list) = new_front_ref;
    }
    return new_front_ref;
}

ref_t list_insert_after (List *list, ref_t ref, val_t val) {
    assert (list);

    if (!is_valid_ref (list, ref)) {
        return List::BAD_REF;
    }

    ref_t new_ref = list_allocate_val (list, val);
    if (new_ref != List::BAD_REF) {
        PREV (list, new_ref) = ref;
        NEXT (list, new_ref) = list->nodes[ref].next;
        
        PREV (list, NEXT (list, ref)) = new_ref; 
        NEXT (list, ref)              = new_ref;
    }
    return new_ref;
}

ref_t list_erase (List *list, ref_t ref) {
    assert (list);

    if (!is_valid_ref (list, ref)) { // alreay erased
        return ref;
    }

    if (ref == HEAD (list)) {
        HEAD (list) = NEXT (list, HEAD (list));
        list_erase_ref (list, ref); 
        return ref;
    } else {
        auto prev_ref = PREV (list, ref);
        if (prev_ref != List::BAD_REF) {
            NEXT (list, prev_ref) = NEXT (list, ref);
            if (NEXT (list, ref) != List::BAD_REF) { // is there a next to tie
                PREV (list, NEXT (list, ref)) = prev_ref;
            }
            list_erase_ref (list, ref); 
            return ref;   
        }
    }
    return List::BAD_REF;
}

bool  is_valid_ref (List *list, ref_t ref) {
    if (ref != List::BAD_REF) {
        return list->nodes[ref].prev != ref;
    }
    return false;
}