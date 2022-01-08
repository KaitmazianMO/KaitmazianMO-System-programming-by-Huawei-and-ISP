/*
 * format: INSTR_CNTX_REPR(cmd, unique_id, CODE(code))
 * 
 * 
 * 
 * 
 * 
 */

#ifndef INSTR_CNTX_REPR
#error "INSTR_CNTX_REPR is requiet"
#endif

#define CODE( ... )  { __VA_ARGS__ }

#define PUSH 

INSTR_CNTX_REPR(add, 0 , 
    CODE(
    )
)

INSTR_CNTX_REPR(sub, 1 , 
    CODE(
    )
)

INSTR_CNTX_REPR(mul, 2 , 
    CODE(
    )
)

INSTR_CNTX_REPR(div, 3 , 
    CODE(
    )
)

INSTR_CNTX_REPR(print, 4 , 
    CODE(
    )
)

INSTR_CNTX_REPR(move, 5 , 
    CODE(
    )
)

INSTR_CNTX_REPR(N, 6, 
    CODE(
    )
)

#undef INSTR_CNTX_REPR
#undef CODE
