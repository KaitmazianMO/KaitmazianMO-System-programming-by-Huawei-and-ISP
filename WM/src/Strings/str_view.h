#ifndef STR_VIEW_H_INCLUDED
#define STR_VIEW_H_INCLUDED

#include <stddef.h>

struct StrView {
    const char *beg;
    size_t      size;
};

inline StrView make_str_view (const char *beg, size_t size) {
    return { .beg = beg, .size = size };
}

#endif // STR_VIEW_H_INCLUDED