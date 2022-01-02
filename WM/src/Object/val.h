#ifndef VALUE_H_INCLUDED_
#define VALUE_H_INCLUDED_

#include <stdint.h>
#include <decimal/decimal>

typedef std::decimal::decimal64 decimal64;
typedef decimal64 decimal_n;
typedef int64_t integer_n;  

integer_n dec_to_int (decimal_n d);
decimal_n int_to_dec (integer_n i);

union Value {
    //string  str_;
    integer_n integer;
    decimal_n decimal;
};

inline integer_n val_int (Value val) { return val.integer; }
inline decimal_n val_dec (Value val) { return val.decimal; }
//inline string val_str (Value val) { return val.str_; }


#endif // VALUE_H_INCLUDED_