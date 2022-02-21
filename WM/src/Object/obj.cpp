#include "obj.h"
#include "err.h"
#include <assert.h>


bool val_is_equal (Value lhs, Value rhs, Tag tag) {
    switch (tag) {
        case INT: {
            return lhs.integer == rhs.integer;
        } break;
        case DEC: {
            return lhs.decimal == rhs.decimal;
        }
    }
    return false;
}

bool obj_is_equal (Object lhs, Object rhs) {
    if (obj_tag (lhs) == obj_tag (rhs)) {
        return val_is_equal (obj_val (lhs), obj_val (rhs), obj_tag (lhs));
    }
    return false;
}

#define CMP_AS( type )\
    if (lhs.val.type <  rhs.val.type) return LESS;  \
    if (lhs.val.type >  rhs.val.type) return MORE;  \
    if (lhs.val.type == rhs.val.type) return EQUAL; \

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