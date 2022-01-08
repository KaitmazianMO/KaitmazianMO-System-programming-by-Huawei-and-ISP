#include "asm.h"
#include "consts.h"
#include "instr.h"
#include "err.h"
#include "log.h"
#include <assert.h>
#include <errno.h>


static inline TokenType  type (Token tok) { return tok.type; };
static inline StrView    name (Token tok) { return tok.name; };

static inline const char *beg  (StrView sv) { return sv.beg; };
static inline size_t      size (StrView sv) { return sv.size; };

static arg_t handle_register (Token tok_reg);
static arg_t handle_constant (Token tok_const, ObjectPool *obj_pool, Code *code, arg_t (*arg_formatter)(arg_t));

typedef int (*cmd_handler_t) (Assembler *assm);

/* handler declaration */
#define INSTR_CNTX_REPR( instr, opcode, ... )\
    int instr ## _handler (Assembler *assm);

#include "../IntructionSetDSL/instr_def.h"

inline int call_handler (Assembler *assm, Opcode opcode) {
    /* handler table initialization */
    #define INSTR_CNTX_REPR( instr, opcode, ... )\
        instr ## _handler, 

    static cmd_handler_t handlers[] = {
        #include "../IntructionSetDSL/instr_def.h"
    }; 
    
    LOG_INFO ("Calling handler for %s with opcode = %d, %d opcodes at all", instr_to_str (opcode), opcode, op_N);
    if (opcode < op_N) {
        return handlers[opcode](assm);
    }
    return handlers[op_N](assm);
}

#define LEX             (assm->lex_)
#define pLEX            (&assm->lex_)
#define LEXLINE         (assm->lex_.nline_) 
#define OBJ_POOL        (assm->obj_pool_)
#define pOBJ_POOL       (&assm->obj_pool_)
#define CODE            (assm->code_)
#define pCODE           (&assm->code_)
#define META            (assm->meta_)
#define pMEATA          (&assm->meta_)
#define CODE_SIZE       (META.code_sec_size_)
#define OBJ_POOL_SIZE   (META.pool_sec_size_)

int asm_init (Assembler *assm, const char *source) {
    assert (assm);
    assert (source);

    *assm = asm_destroyable(); 

    if (lex_init (pLEX, source) && /* lex doesn't have any resources */
        obj_pool_init (pOBJ_POOL, G_OBJ_TABLE_INITIAL_SIZE) && 
        code_init (pCODE, G_CODE_INITIAL_SIZE)) {
        return 1;
    } /* smth wasn't constructed */
    
    asm_free (assm);
    return 0;
}

int asm_free (Assembler *assm) {
    if (obj_pool_free (pOBJ_POOL) && code_free (pCODE)) {
        *assm = asm_destroyable();
        return 1;
    }
    return 0;
}

Assembler asm_destroyable() {
    Assembler assembler = {};
    assembler.obj_pool_ = obj_pool_destroyable();
    assembler.code_     = code_destroyable();
    return assembler;
}

int asm_translate (Assembler *assm) {
    assert (assm);

    for (Token tok = lex_get_tok (pLEX); type (tok) != FINAL; tok = lex_get_tok (pLEX)) {
        if (type (tok) == IDENTIFIER) {
            LOG_INFO ("Start translating identifier \'%.*s\' from %zu line",
                (int)tok.name.size, tok.name.beg, tok.nline);
            auto found = find_reserved_command (name (tok));
            if (found.id != op_N) {
                LOG_INFO ("Found reserved word (%s:%d)", found.name, found.id)
                if (call_handler (assm, found.id) == 0) {
                    lex_skip_line (pLEX);
                } 
            } else {
                assert (false && "Unreserved identifyier will be later");
            }
        } else {
            ERROR ("Unexpected token \'%.*s\' at %zu",
                size (name (tok)), name (tok), LEXLINE);
            lex_skip_line (pLEX);
        } 
    }
    return 1;
}

int asm_create_executable_file (Assembler *assm, const char *file_name) {
    assert (assm);
    assert (file_name);

    FILE *exfile = fopen (file_name, "wb");
    if (exfile) {
        Meta exmeta = {};

        exmeta.pool_sec_size_ = obj_pool_dump (exfile);
        exmeta.code_sec_size_ = code_dump (exfile);
        // .fsize = write (meta) + write (obj_pool) + write (code) 

        // meta
        // obj_poop
        // code

    }
}

arg_t get_reg_num (StrView sv) {
    arg_t regn = G_INSTR_ARG_INVALID;
    if (sv.beg[0] != 'R') {
        char *nend = NULL;
        errno = 0;
        arg_t num = (arg_t)strtol (beg (sv) + 1, &nend, 10);
        regn = arg_to_R (num);
        if (regn == G_INSTR_ARG_INVALID || errno == ERANGE || num < 0) {
            error ("Wrong register number " REGREF_F ", "
                   "there is only %zu awailable registers", num, G_INSTR_N_REGISTERS);
        }

        if (sv.beg + sv.size != nend) { 
            error ("Extra cahracters at et the end of register \'%.*s\'", (int)sv.size, sv.beg);
        }
    } else {
        error ("Expected register");
    }

    return regn;
}

Object make_object (Token tok) {
    Object obj = { .tag = VOID };
    if (tok.type == INT_NUMBER) {
        obj.tag = INT;
        obj.val.integer = tok.num.integer;
        LOG_INFO ("Maked an integer number object %.*s", tok.name.size, tok.name.beg);
    } else if (tok.type == DEC_NUMBER) {
        obj.tag = DEC;
        obj.val.decimal = tok.num.decimal;
        LOG_INFO ("Maked an decimal number object %.*s", tok.name.size, tok.name.beg);
    } else {
        ERROR ("Unknown token type %d", tok.type);
    }

    return obj;
}

int handle_arithmetic (Assembler *assm, Opcode ar_code) {
    arg_t R{};
    arg_t A{};
    arg_t B{};

    auto tokR = lex_get_tok (pLEX);
    if (tok_is_identifier (tokR)) {
        R = handle_register (tokR);
    } else {        
        error ("Expected register identtifyer after the command %s at line %zu", 
            instr_to_str (ar_code), tokR.nline); 
    }   

    auto tokA = lex_get_tok (pLEX);
    if (tok_is_identifier (tokA)) {
        A = handle_register (tokA);
    } else if (tok_is_const (tokA)) {
        A = handle_constant (tokA, pOBJ_POOL, pCODE, arg_to_A);
    } else {
        error ("Expected register or constant identtifyer after the command %s at line %zu", 
            instr_to_str (ar_code), tokR.nline); 
    }
     
    auto tokB = lex_get_tok (pLEX);
    if (tok_is_identifier (tokB)) {
        B = handle_register (tokB);
    } else if (tok_is_const (tokB)) {
        B = handle_constant (tokB, pOBJ_POOL, arg_to_B);
    } else {
        error ("Expected register or constant identtifyer after the command %s at line %zu", 
            instr_to_str (ar_code), tokR.nline); 
    }

    if (R != G_INSTR_ARG_INVALID &&
        A != G_INSTR_ARG_INVALID && 
        B != G_INSTR_ARG_INVALID) {
        return code_write_RAB (pCODE, ar_code, R, A, B);
    }
    
    return 0;
}

arg_t handle_register (Token tok_reg) {
    assert (tok_is_identifier (tok_reg));
    arg_t R = G_INSTR_ARG_INVALID;
    if (tok_reg.name.beg[0] == 'R') {
        char *nend = NULL;
        errno = 0;
        arg_t num = (arg_t)strtol (tok_reg.name.beg + 1, &nend, 10);
        R = arg_to_R (num);
        if (R == G_INSTR_N_REGISTERS || errno == ERANGE) {
            error ("Wrong register number " REGREF_F ", "
                   "there is only %zu awailable registers", num, G_INSTR_N_REGISTERS);
        }

        if (!isspace (*nend) || *nend != '\0') { 
            error ("Extra cahracters at et the end of register \'%.*s\'", (int)sv.size, sv.beg);
        }
    } else {
        error ("Expected register");
    }

    return R;    
}

static arg_t load_const_in_reg (Code *code, pool_idx cnst, arg_t reg);

arg_t handle_constant (Token tok_const, ObjectPool *obj_pool, Code *code, arg_t (*arg_formatter)(arg_t)) {
    assert (tok_is_const (tok_const));
    assert (obj_pool);
    assert (arg_formatter);

    arg_t ref = G_INSTR_ARG_INVALID;
    auto const_obj = make_object (tok_const);
    if (const_obj.tag != VOID) {
        auto const_idx = obj_pool_insert (obj_pool, const_obj);
        if (const_idx != OBJ_POOL_BAD_IDX) {
            ref = arg_formatter (const_idx);
            if (ref == G_INSTR_ARG_INVALID) { /* try to load const in reg before call operation */
                ref = load_const_in_reg (code, const_idx, G_INSTR_N_REGISTERS - 1);
            }
        }
    }

    return ref;
}

arg_t load_const_in_reg (Code *code, pool_idx cnst, arg_t reg) {
    assert (code);
    
    auto R = arg_to_R (reg);
    if (R == G_INSTR_ARG_INVALID) {
        fatal ("Bad register for constant loading");
        return G_INSTR_ARG_INVALID;
    }
    auto C = arg_to_C (cnst);
    if (C == G_INSTR_ARG_INVALID) {
        error ("Too many constants, there is more than %zu constants in the pool", cnst);
        return G_INSTR_ARG_INVALID;
    }
    
    return code_write_RC (code, op_move, R, C) ? R : G_INSTR_ARG_INVALID;
}

arg_t handle_constant_A (Assembler *assm, arg_t A) {
    if (G_INSTR_ARG_A_MAX < A && A != G_INSTR_ARG_INVALID) {
        if (code_wtite_RC (pCODE, op_move, G_INSTR_N_REGISTERS - 1, A)) {
            A = G_INSTR_N_REGISTERS - 1;
        } else {
            error ("To many constatns")
            A = G_INSTR_ARG_INVALID;
        }
    }

    return A;
}

int add_handler (Assembler *assm) {
    return handle_arithmetic (assm, op_add);
}

int sub_handler (Assembler *assm) {
    return handle_arithmetic (assm, op_sub);
}

int mul_handler (Assembler *assm) {
    return handle_arithmetic (assm, op_mul);
}

int div_handler (Assembler *assm) {
    return handle_arithmetic (assm, op_div);
}

int move_handler (Assembler *assm) {
    
}

int print_handler (Assembler *assm) {

}

int N_handler (Assembler *assm) {
    error ("Unknown opcode was executed");
    return 0;
}

#undef LEX           
#undef pLEX          
#undef OBJ_POOL      
#undef pOBJ_POOL     
#undef CODE          
#undef pCODE         
#undef META          
#undef pMEATA        
#undef CODE_SIZE     
#undef OBJ_POOL_SIZE 