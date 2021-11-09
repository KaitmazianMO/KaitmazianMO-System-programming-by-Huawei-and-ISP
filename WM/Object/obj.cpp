#include "obj.h"
#include <assert.h>

int val_is_equal (Value lhs, Value rhs, Tag tag) {
    switch (tag)
    {
        case INT: {
            return (lhs.integer == rhs.integer) ? EQUAL : NOT_EQUAL;
        } break;
        case DEC: {
            return (lhs.decimal == rhs.decimal) ? EQUAL : NOT_EQUAL;
        }
        default:
            return UNCOMPARABLE;
    }
}

CompareResult val_is_less_or_more (Value lhs, Value rhs, Tag tag) {
    switch (tag)
    {
        case INT: {
            if (lhs.integer < rhs.integer) return LESS;
            if (lhs.integer > rhs.integer) return MORE;
        } break;
        case DEC: {
            if (lhs.decimal < rhs.decimal) return LESS;
            if (lhs.decimal > rhs.decimal) return MORE;
        }
        case STR :{
            assert (false && "Str is not soported now");
        } break;

        case VOID: {
            
        }
    }
    return UNCOMPARABLE;    
}

bool obj_is_equal (Object lhs, Object rhs) {
    if (obj_tag (lhs) == obj_tag (rhs)) {
        return val_is_equal (obj_val (lhs), obj_val (rhs), obj_tag (lhs)) == EQUAL;
    }
    return false;
}

CompareResult obj_compare (Object lhs, Object rhs) {
    if (obj_tag (lhs) == obj_tag (rhs)) {
        if (val_is_equal (obj_val (lhs), obj_val (rhs), obj_tag (lhs)) == EQUAL) {
            return EQUAL;
        } else {
            return val_is_less_or_more (obj_val (lhs), obj_val (rhs), obj_tag (lhs));
        }
    }
    return UNCOMPARABLE;
}