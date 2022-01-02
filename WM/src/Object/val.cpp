#include "val.h"

integer_n dec_to_int (decimal_n d) {
    return  decimal_to_long_long (d);  
}

decimal_n int_to_dec (integer_n i) {
    return (decimal_n)i;
}