#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include <assert.h>

#include "buffer.h"
#include "../Utilities/c_dangeon.h"
#include "../Utilities/Log/log.h"
#include "../Utilities/Log/gcc_trace.h"

#ifndef T
#define T int
#endif

#define VECTOR_T DECLARE (vector, T)

enum VEC_ERROR
{
    VEC_SUCCESS = 0,
    VEC_ALLOCATION_FAILED = 256, VEC_RESIZING_FAILED, 
    VEC_LOGIC_ERROR, VEC_OUT_OF_RANGE,
};

struct VECTOR_T
{
    Buffer data;
    size_t size;
};

//#define VEC_ELEM( idx )   (*buf_offset (_this->buf, _this->size))
#define VEC_SIZE          (_this->size)
#define VEC_CAP           (_this->data.capacity) 
#define VEC_PDATA         (&_this->data)
#define VEC_VERIFY        { assert (_this); assert (VEC_SIZE <= VEC_CAP); }
__attribute__ ((unused))
static VEC_ERROR DECLARE (vec_ctor, T) (VECTOR_T *_this, size_t cap)
{
    assert (_this);

    LOG_MSG (LOG, "FUNCING VECTOR CONSTUCTING");

    int err = buf_ctor (VEC_PDATA, cap, sizeof (T));
    RETURN_IF_TRUE (err, VEC_ALLOCATION_FAILED)

    _this->size = cap;
    return VEC_SUCCESS;
}

__attribute__ ((unused))
static VEC_ERROR DECLARE (vec_push_back, T) (VECTOR_T *_this, T val)
{
    VEC_VERIFY

    int err = 0;
    if (VEC_SIZE == VEC_CAP)
    {
        err = buf_resize (VEC_PDATA, (VEC_SIZE + 1) * 2);
        RETURN_IF_TRUE (err, VEC_RESIZING_FAILED)
    }

    char *pelem = NULL;
    buf_offset (VEC_PDATA, &pelem, VEC_SIZE);
    RETURN_IF_TRUE (pelem == NULL, VEC_LOGIC_ERROR)
    
    ++VEC_SIZE;
    *(T *)pelem = val;
    return VEC_SUCCESS;
} 

__attribute__ ((unused))
static VEC_ERROR DECLARE (vec_get_elem, T) (VECTOR_T *_this, size_t idx, T *pval)
{
    VEC_VERIFY
    assert (pval);

    char *pelem = NULL;
    buf_offset (VEC_PDATA, &pelem, idx);
    RETURN_IF_TRUE (pelem == NULL, VEC_OUT_OF_RANGE)

    *pval = *(T *)pelem;
    return VEC_SUCCESS;
}

__attribute__ ((unused))
static VEC_ERROR DECLARE (vec_set_elem, T) (VECTOR_T *_this, size_t idx, T val)
{
    VEC_VERIFY

    char *pelem = NULL;
    buf_offset (VEC_PDATA, &pelem, idx);
    RETURN_IF_TRUE (pelem == NULL, VEC_OUT_OF_RANGE)

    *(T *)pelem = val;
    return VEC_SUCCESS;
}

__attribute__ ((unused))
static VEC_ERROR DECLARE (vec_dtor, T) (VECTOR_T *_this)
{
    VEC_VERIFY

    VEC_SIZE = 0;
    
    return (VEC_ERROR)buf_dtor (VEC_PDATA);    
}

#undef T
#undef VEC_ELEM
#undef VEC_SIZE       
#undef VEC_CAP        
#undef VEC_VERIFY    

#endif