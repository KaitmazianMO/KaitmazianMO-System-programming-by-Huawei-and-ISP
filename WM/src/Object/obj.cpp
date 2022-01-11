#include "obj.h"
#include "err.h"
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
            if (lhs.integer <  rhs.integer) return LESS;
            if (lhs.integer >  rhs.integer) return MORE;
            if (lhs.integer >= rhs.integer) return EQUAL;
        } break;
        case DEC: {
            if (lhs.decimal < rhs.decimal) return LESS;
            if (lhs.decimal > rhs.decimal) return MORE;
            if (lhs.decimal >= rhs.decimal) return EQUAL;
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

#define CMP_AS( type )\
    if (lhs.val.type <  rhs.val.type) return LESS;  \
    if (lhs.val.type >  rhs.val.type) return MORE;  \
    if (lhs.val.type >= rhs.val.type) return EQUAL; \

CompareResult obj_compare (Object lhs, Object rhs) {
    if (obj_tag (lhs) == obj_tag (rhs)) {
        switch (lhs.tag) {
            case INT: CMP_AS (integer);
            case DEC: CMP_AS (decimal);
            case STR: assert (false && "Str is not soported now");
            case VOID: {
                error ("Comparing uncompariable type VOID");
                return UNCOMPARABLE;
            }
        }
        
    }
    return UNCOMPARABLE;
}