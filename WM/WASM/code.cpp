#include "code.h"
#include "../Error/err.h"
#include <assert.h>

#define INSTR  (code->instr_)
#define SIZE   (code->size_)
#define POS    (code->pos_)

int verify_reg (reg_t reg);
int verify_regref (regref_t regref);
int verify_ref (ref_t ref);

int code_init (Code *code, size_t size) {
    assert (code);
    assert (size);

    INSTR = (Instruction *)calloc (size, sizeof (Instruction));
    if (INSTR) {
        SIZE = size;
        POS = 0;
        return 1;
    }
    return 0;
}

int code_free (Code *code) {
    assert (code);

    free (INSTR);
    SIZE = 0;
    POS = 0;
    return 1;
}

int code_grow (Code *code) {
    assert (code);

    auto new_code = (Instruction *)realloc (INSTR, 2*SIZE*sizeof(Instruction));
    if (new_code) {
        INSTR = new_code;
        SIZE *= 2;
        return 1;
    }
    return 0;
}

int code_push (Code *code, Instruction inst) {
    assert (code);

    if (SIZE <= POS) {
        if (code_grow (code)) {
            INSTR[POS++] = inst;
            return 1;
        }
        fatal ("There is no memory for the code, allocated %zu(%zu bytes)", SIZE, SIZE*sizeof(Instruction));
        return 0;
    }

    INSTR[POS++] = inst;
    return 1;
}

#define write_instr_CMD( instr, opcode ) \
    (instr.cmd = opcode)
#define write_instr_R( instr, R )  \
    ((instr.args) |= (R ))
#define write_instr_A( instr, A )  \
    ((instr.args) |= (A << G_INSTR_REG_NBITS))
#define write_instr_B( instr, B )  \
    ((instr.args) |= (A << (G_INSTR_REG_NBITS + G_INSTR_AB_NBITS)))

int code_write_RAB (Code *code, Opcode opcode, reg_t R, regref_t A, regref_t B){
    assert (code);

    if (!verify_reg (R)) {
        return 0;
    } else if ( !(verify_ref (A) && verify_ref (B)) ) {
        return 0;
    }

    Instruction instr{};
    write_instr_CMD (instr, opcode);
    write_instr_R (instr, R);
    write_instr_A (instr, A);
    write_instr_B (instr, B);
    return code_push (code ,instr);
}

int code_wtite_C (Code *code, Opcode opcode, ref_t C){
    assert (code);

    if (!verify_ref (C)) {
        return 0;
    }

    Instruction instr = {};
    write_instr_CMD (instr, opcode);
    instr.cmd = C;
    return code_push (code ,instr);
}

int verify_reg (reg_t reg) {
    if (reg > G_INSTR_REG_T_MAX) {
        error ("Incorrect register number (%d) ", reg);
        return 0;
    }
    return 1;
}

int verify_ref (ref_t ref) {
    if (ref > G_INSTR_REGREF_T_MAX) {
        error ("Too many constants(more than %zu), overflow of constant pool", G_INSTR_REF_T_MAX);
        return 0;
    }
    return 1;
}
