#ifndef META_H_INCLUDED
#define META_H_INCLUDED

#include <stddef.h>

struct Meta {
    // ver_t version_;   // read first
    size_t fsize_;  // read second
    // int hash_;  // check before execute
    size_t code_sec_;
    size_t code_sec_size_;
    size_t pool_sec_;
    size_t pool_sec_size_;
    size_t stack_size_;
};

#endif // META_H_INCLUDED