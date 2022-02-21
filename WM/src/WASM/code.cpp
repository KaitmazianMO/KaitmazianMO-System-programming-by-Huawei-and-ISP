#include "code.h"
#include "../Error/err.h"
#include <assert.h>

#define INSTR  (code->instr_)
#define SIZE   (code->size_)
#define POS    (code->pos_)

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

size_t code_dump (const Code *code, FILE *file) {
    assert (code);
    assert (file);

    size_t wrote =  fwrite (INSTR, sizeof (INSTR[0]), SIZE, file);
    return wrote; 
}

size_t code_size (const Code *code) {
    assert (code);

    return SIZE;
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
        fatal ("There is no memory for the code, allocated %zu instr(%zu bytes)", SIZE, SIZE*sizeof(Instruction));
        return 0;
    }

    INSTR[POS++] = inst;
    return 1;
}

int code_write_RAB (Code *code, Opcode opcode, arg_t R, arg_t A, arg_t B) {
    assert (code);
    assert (R != G_INSTR_ARG_INVALID);
    assert (A != G_INSTR_ARG_INVALID);
    assert (B != G_INSTR_ARG_INVALID);
    assert (opcode < op_N);

    Instruction instr{};
    instr.RAB.opcode = opcode;
    instr.RAB.R      = R;
    instr.RAB.A      = A;
    instr.RAB.B      = B;
    return code_push (code ,instr);
}

int code_write_RC (Code *code, Opcode opcode, arg_t R, arg_t C) {
    assert (code);
    assert (R != G_INSTR_ARG_INVALID);
    assert (C != G_INSTR_ARG_INVALID);

    Instruction instr = {};
    instr.RC.opcode = opcode;
    instr.RC.R      = R;
    instr.RC.C      = C;
    return code_push (code ,instr);
}

int code_write_D (Code *code, Opcode opcode, arg_t D) {
    assert (code);
    assert (D != G_INSTR_ARG_INVALID);

    Instruction instr = {};
    instr.D.opcode = opcode;
    instr.D.D      = D;
    return code_push (code ,instr);
}
