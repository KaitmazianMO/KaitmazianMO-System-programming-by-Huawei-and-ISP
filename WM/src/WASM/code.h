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
size_t code_size (Code *code);
size_t code_dump (FILE *file);

inline Code code_destroyable() { return Code{ .instr_ = NULL, .size_ = 0 }; }

/* R - register | A,B - register of reference | C - reference */
int code_write_RAB (Code *code, Opcode opcode, arg_t R, arg_t A, arg_t B);
int code_write_RC (Code *code, Opcode opcode, arg_t R, arg_t C);
int code_write_D (Code *code, Opcode opcode, arg_t D);

#endif // CODE_H_INCLUDED