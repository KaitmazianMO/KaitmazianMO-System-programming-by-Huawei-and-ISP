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

int code_wtite_RC (Code *code, Opcode opcode, arg_t R, arg_t C) {
    assert (code);

    if (!verify_ref (C)) {
        return 0;
    }

    Instruction instr = {};
    write_instr_CMD (instr, opcode);
    instr.cmd = C;
    return code_push (code ,instr);
}

int code_write_D (Code *code, Opcode opcode, arg_t D) {

}
