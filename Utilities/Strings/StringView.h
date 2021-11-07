#ifndef STRING_VIEW_H_INCLUDED
#define STRING_VIEW_H_INCLUDED

#include <stddef.h>

struct StringView {
    const char *beg_;
    size_t size_;
};

inline StringView make_string_view (const char *beg, size_t size) {
    return StringView { .beg_ = beg, .size_ = size };
}

#endif // STRING_VIEW_H_INCLUDED