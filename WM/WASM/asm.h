#ifndef ASM_H_INCLUDED
#define ASM_H_INCLUDED

#include "lex.h"
#include "../Meta/meta.h"
#include "../Instruction/instr.h"
#include "../ObjectPool/obj_pool.h"
#include "code.h"

struct Assembler {
    Meta meta_;
    Lexer lex_;
    Code code_;
    ObjectPool obj_pool_;
};

int asm_init (Assembler *assm, const char *source);
int asm_translate (Assembler *assm);
int asm_create_execute_file (Assembler *assm, const char file_name); 
Assembler asm_destroyable();
int asm_free (Assembler *assm);

#endif // ASM_H_INCLUDED