#include "list.h"
#include <assert.h>

#define LNEXT  (this_->next)
#define LHEAD (this_->head_ref)
#define LBACK  (this_->ref_back)
#define LPOOL  (this_->pool)
#define LSIZE  (this_->size)

ref_t find_first_free_ref (List *this_) {
    for (size_t i = 0; i < LIST_POOL_SIZE; ++i) {
        if (this_->next[i] == i) {
            return i;
        }
    }
    return LIST_BAD_REF;
}

ref_t list_allocate_val (List *this_, val_t val) {
    auto ref = find_first_free_ref (this_);
    if (ref != LIST_BAD_REF) {
        LPOOL[ref] = val;
        ++LSIZE;
    }

    return ref;
}

void list_erase_ref (List *this_, ref_t ref) {
    LNEXT[ref] = ref;
    --LSIZE;
}

ref_t list_find_prev (List *this_, ref_t ref) {
    int i = LHEAD;
    if (LNEXT[i] == i || LNEXT[ref] == ref) {
        return LIST_BAD_REF;
    }

    while (LNEXT[i] != ref) {
        i = LNEXT[i];
    }
    return i;
}

int list_init (List *this_) {
    assert (this_);
    LHEAD = LIST_BAD_REF;
    LSIZE = 0;
    for (int i = 0; i < LIST_POOL_SIZE; ++i) {
        LNEXT[i] = i;
    }

    return 1;
}

int list_free (List *this_) {
    assert (this_);
    return 1;
}

ref_t list_insert_front (List *this_, val_t val) {
    assert (this_);

    auto new_front_ref = list_allocate_val (this_, val);
    if (new_front_ref != LIST_BAD_REF) {
        LNEXT[new_front_ref] = LHEAD;
        LHEAD = new_front_ref;
    }
    return new_front_ref;
}

ref_t list_insert_after (List *this_, ref_t ref, val_t val) {
    assert (this_);

    auto new_ref = list_allocate_val (this_, val);
    if (new_ref != LIST_BAD_REF) {
        LNEXT[new_ref] = LNEXT[ref];
        LNEXT[ref] = new_ref;
    }
    return new_ref;
}

ref_t list_erase (List *this_, ref_t ref) {
    assert (this_);

    if (ref == LHEAD) {
        LHEAD = LNEXT[LHEAD];
        list_erase_ref (this_, ref); 
        return LHEAD;
    } else {
        auto prev_ref = list_find_prev (this_, ref);
        if (prev_ref != LIST_BAD_REF) {
            LNEXT[prev_ref] = LNEXT[ref];
            list_erase_ref (this_, ref); 
            return LNEXT[prev_ref];   
        }
    }
    return LIST_BAD_REF;
}
