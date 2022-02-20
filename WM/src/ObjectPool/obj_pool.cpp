#include "obj_pool.h"
#include "../Error/err.h"
#include <stdlib.h>
#include <assert.h>

#define OBJS      (this_->objs)
#define NOBJS     (this_->nobjs)
#define CAPACITY  (this_->capacity) 

constexpr size_t grow_coef = 2u;
int obj_pool_grow (ObjectPool *this_);

int obj_pool_init (ObjectPool *this_, size_t size) {
    assert (this_);
    assert (size);

    OBJS = (Object *)calloc (size, sizeof (OBJS[0]));
    if (OBJS) {
        NOBJS = 0;
        CAPACITY = size;
        return 1;
    }

    return 0;
}

int obj_pool_free (ObjectPool *this_) {
    assert (this_);

    free (OBJS);
    *this_ = obj_pool_destroyable();
    return 1;
}

pool_idx obj_pool_look_up (const ObjectPool *this_, Object obj) {
    assert (this_);

    for (size_t i = 0; i < NOBJS; ++i) {
        if (OBJS[i].tag ==  obj.tag) {
            if (obj_is_equal (OBJS[i], obj)) {
                return i;
            }
        }
    }
    return OBJ_POOL_BAD_IDX;
}

pool_idx obj_pool_insert (ObjectPool *this_, Object obj) {
    assert (this_);

    auto found = obj_pool_look_up (this_, obj);
    if (found != OBJ_POOL_BAD_IDX) { /* obj is in the pool */
        return found;
    }
    
    if (NOBJS == CAPACITY) { 
        if (obj_pool_grow (this_)) {
            OBJS[NOBJS++] = obj;
            return NOBJS-1;
        } else {
            error ("No memory for new object allocation in pool, allocated %zu constants", NOBJS);
            return OBJ_POOL_BAD_IDX;
        }
    }
    OBJS[NOBJS++] = obj;
    return NOBJS-1;    
}

Object obj_pool_get (const ObjectPool *this_, pool_idx idx) {
    assert (this_);
    assert (idx != OBJ_POOL_BAD_IDX);

    if (idx <= NOBJS) {
        return OBJS[idx];
    }
    error ("appeal out of pool range");
    return Object {.tag = VOID};
}

size_t obj_pool_dump (const ObjectPool *this_, FILE *file) {
    assert (this_);
    assert (file);

    size_t wrote =  fwrite (OBJS, sizeof (OBJS[0]), NOBJS, file);
    return wrote; 
}

size_t obj_pool_size (const ObjectPool *this_) {
    assert (this_);
    return NOBJS;
}

int obj_pool_grow (ObjectPool *this_) {
    assert (this_);

    Object *new_objs = (Object *)realloc (OBJS, 2*CAPACITY);
    if (new_objs) {
        OBJS = new_objs;
        CAPACITY *= grow_coef; 
        return 1;
    }
    return 0;
}