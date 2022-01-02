#ifndef OBJ_H_INCLUDED
#define OBJ_H_INCLUDED

#include "val.h"

enum Tag {
    INT,
    DEC,
    STR,
    VOID,
};

struct Object {
    Tag tag;
    Value val;
};

enum CompareResult {
    EQUAL,
    NOT_EQUAL,
    LESS,
    MORE,
    UNCOMPARABLE,
};

inline Tag   obj_tag (Object obj) { return obj.tag; }
inline Value obj_val (Object obj) { return obj.val; }
    
bool obj_is_equal (Object lhs, Object rhs);
CompareResult obj_compare (Object lhs, Object rhs);

#endif // OBJ_H_INCLUDED