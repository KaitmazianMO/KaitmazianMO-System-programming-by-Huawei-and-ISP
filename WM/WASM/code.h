#ifndef CODE_H_INCLUDED
#define CODE_H_INCLUDED

#include "../Instruction/instr.h"
#include <stddef.h>

struct Code {
    Instruction *instr_;
    size_t pos_;
    size_t size_;
};

int code_init (Code *code, size_t size);
int code_free (Code *code);
inline Code code_destroyable() { return Code{ .instr_ = NULL, .size_ = 0 }; }

/* R - register | A,B - register of reference | C - reference */
int code_write_RAB (Code *code, Opcode opcode, reg_t R, regref_t A, regref_t B);
int code_write_A (Code *code, Opcode opcode, regref_t A);
int code_wtite_C (Code *code, Opcode opcode, ref_t C);

#endif // CODE_H_INCLUDED