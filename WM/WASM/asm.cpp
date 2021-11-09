#include "asm.h"
#include "../Constants/consts.h"
#include "../Instruction/instr.h"
#include "../Error/err.h"
#include <assert.h>
#include <errno.h>

inline TokenType  type (Token tok) { return tok.type; };
inline StringView name (Token tok) { return tok.name; };

inline const char *beg  (StringView sv) { return sv.beg_; };
inline size_t      size (StringView sv) { return sv.size_; };

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
    
    printf ("opcode to call = %d\n", opcode);
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
            printf ("get identigyer \'%.*s\'\n", (int)tok.name.size_, tok.name.beg_);
            auto found = find_reserved_command (name (tok));
            printf ("found.name = \'%s\'\n", found.name);
            printf ("found.id   = \'%d\'\n", found.id);
            printf ("calling handler of id\n");
            if (found.id != op_N) {
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

reg_t to_register (StringView sv) {
    char *nend = NULL;
    errno = 0;
    reg_t num = (reg_t)strtol (beg (sv) + 1, &nend, 10);
    if (instr_to_reg (num) > G_INSTR_N_REGISTERS || errno == ERANGE || num < 0) {
        error ("Wrong register number " REGREF_F ", "
               "there is only %zu awailable registers", num, G_INSTR_REG_T_MAX);
        return -1;
    }

    if (sv.beg_ + sv.size_ != nend) { 
        error ("Extra cahracters at et the end of register \'%.*s\'", (int)sv.size_, sv.beg_);
        return -1;
    }

    return instr_to_reg (num);
}

Object make_object (Token tok) {
    Object obj = { .tag = VOID };
    if (tok.type == INT_NUMBER) {
        obj.tag = INT;
        obj.val.integer = tok.num.integer;
    } else if (tok.type == DEC_NUMBER) {
        obj.tag = DEC;
        obj.val.decimal = tok.num.decimal;
    } else {
        ERROR ("Unknown token type %d", tok.type);
    }

    return obj;
}

#define EXPECT( tag_expect, opcode )    \
    if (type (tok) != tag_expect) { \
        error ("Expected " #tag_expect " after the command %s at line %zu", \
         instr_to_str (opcode), LEXLINE);   \
         return 0;  \
    }   \
  
#define EXPECT3( tag_expect1, tag_expect2, tag_expect3, opcode )    \
    if (type (tok) != tag_expect1 && type (tok) != tag_expect2 && type (tok) != tag_expect3) { \
        error ("Expected %s, %s or %s"  \
            " after the command %s at line %zu", \
            #tag_expect1, #tag_expect2, #tag_expect3,   \
            instr_to_str (opcode), LEXLINE);   \
        return 0;  \
    }   \

#define HANDLE_AS_REGISTER( reg )   \
    if (beg (name (tok))[0] == 'R') {   \
        A = to_register (name (tok));   \
    } else {    \
        error ("Expected register. Registers names start with R");  \
        return 0;   \
    }   \

#define HADNLE_AS_REF( AB, ref ) \
    if (ref == OBJ_POOL_BAD_IDX) {  \
        return 0;   \
    }   \
    AB = instr_to_ref (ref);    \
    if (AB > G_INSTR_REF_T_MAX) {  \
        ERROR ("Unsopportable number of references (max:%d) will be fixed later", G_INSTR_REF_T_MAX - G_INSTR_REG_T_MAX); \
        return 0;   \
    }   \

#define HANDLE_AB( A )  \
    if (type (tok) == IDENTIFIER) { \
        HANDLE_AS_REGISTER (R); \
    } else { /* number */   \
        auto obj = make_object (tok);   \
        if (obj.tag != VOID) {  \
            ref_t ref = obj_pool_insert (pOBJ_POOL, obj);   \
            HADNLE_AS_REF (A, ref); \
        } else {    \
            return 0;   \
        }   \
    }   \

int handle_arithmetic (Assembler *assm, Opcode ar_code) {
    auto tok = lex_get_tok (pLEX);
    reg_t R{};
    regref_t A{};
    regref_t B{};

    EXPECT (IDENTIFIER, ar_code);
    HANDLE_AS_REGISTER (R);

    tok = lex_get_tok (pLEX);
    EXPECT3 (IDENTIFIER, DEC_NUMBER, INT_NUMBER, ar_code);
    HANDLE_AB (A);

    tok = lex_get_tok (pLEX);
    EXPECT3 (IDENTIFIER, DEC_NUMBER, INT_NUMBER, ar_code);    
    HANDLE_AB (B);

    return code_write_RAB (pCODE, ar_code, R, A, B);
    
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