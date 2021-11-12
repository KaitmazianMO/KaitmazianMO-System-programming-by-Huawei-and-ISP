#ifndef VALUE_H_INCLUDED
#define VALUE_H_INCLUDED

#include <stdio.h>
#include <stddef.h>
#include <stdint.h> 
#include <cmath>
#include "WASM/lex.h"
#include "Instruction/instr.h"
#include "WASM/tok.h"
#include "WASM/asm.h"
#include "Error/err.h"
#include <assert.h>

#define write_CMD( instr, opcode ) \
    (instr.cmd = opcode)
#define write_instr_R( instr, R )  \
    ((instr.args) |= (R ))
#define write_instr_A( instr, A )  \
    ((instr.args) |= (A << G_INSTR_REG_NBITS))
#define write_instr_B( instr, B )  \
    ((instr.args) |= (A << (G_INSTR_REG_NBITS + G_INSTR_AB_NBITS)))

Instruction RAB (Opcode opcode, reg_t R, regref_t A, regref_t B) {
    Instruction instr{};
    write_CMD (instr, opcode);
    write_instr_R (instr, R);
    write_instr_A (instr, A);
    write_instr_B (instr, B);
    return instr;
}

int main() {   

    //print (G_INSTR_NBITS);
    //print (G_INSTR_OPCODE_NBITS);
    //print (G_INSTR_ARG_NBITS);
    //print (G_INSTR_REG_NBITS);
    //print (G_INSTR_BIN_OP_ARG_NBITS);
    //print ((1u << G_INSTR_OPCODE_NBITS));
    //print (op_N);


    //char src[] = "push rax rbx  \n"
    //             "pop rbx   \n"       
    //             "\t push 1 rbx  \n"
    //             " add  rax 0X1.0 2.5 \n"
    //             "\t\t  pop rbx   #im a comment!\n"
    //             "metka:\n"
    //             "\n"
    //             " \n"       
    //             "\t\n"
    //             "#comment line\n"
    //             "laugh"
    //             "\n1.00";

    char src[] = "div R1 R2 R3";

    Assembler ass = {};
    asm_init (&ass, src);
    printf ("asm inited\n");
    asm_translate (&ass);

    auto instr = RAB (op_div, 1, 2, 3);
    printf ("cmd  = %d\n", instr.cmd);
    printf ("args = %x\n", instr.args);


printf ("RAB:\n");
    for (size_t i = 0; i < G_INSTR_NBITS; ++i) {
        printf ("%3zu", i);
    } printf ("\n");
    for (size_t i = 0; i < G_INSTR_OPCODE_NBITS; ++i) {
        printf ("%3d", (instr.cmd >> i) & 0x1);
    } 

    for (size_t i = 0; i < G_INSTR_ARG_NBITS; ++i) {
        printf ("%3d", (instr.args >> i) & 0x1);
    } printf ("\n");

printf ("asm:\n");
    instr = ass.code_.instr_[0];
    for (size_t i = 0; i < G_INSTR_NBITS; ++i) {
        printf ("%3zu", i);
    } printf ("\n");
    for (size_t i = 0; i < G_INSTR_OPCODE_NBITS; ++i) {
        printf ("%3d", (instr.cmd >> i) & 0x1);
    } ;

    for (size_t i = 0; i < G_INSTR_ARG_NBITS; ++i) {
        printf ("%3d", (instr.args >> i) & 0x1);
    } printf ("\n");

printf ("new cmd foramat(size = %zu):\n", sizeof (cmd));
    cmd comand = {};
    comand.RAB.opcode = op_div;
    comand.RAB.R = 1;
    comand.RAB.A = 2;
    comand.RAB.B = 3;
    for (size_t i = 0; i < G_INSTR_NBITS; ++i) {
        printf ("%3zu", i);
    } printf ("\n");
    for (size_t i = 0; i < G_INSTR_OPCODE_NBITS; ++i) {
        printf ("%3d", (comand.RAB.opcode >> i) & 0x1);
    } ;

    for (size_t i = 0; i < G_INSTR_REG_NBITS; ++i) {
        printf ("%3d", (comand.RAB.R >> i) & 0x1);
    } ;    

    for (size_t i = 0; i < G_INSTR_AB_NBITS; ++i) {
        printf ("%3d", (comand.RAB.A >> i) & 0x1);
    } ;    

    for (size_t i = 0; i < G_INSTR_AB_NBITS; ++i) {
        printf ("%3d", (comand.RAB.B >> i) & 0x1);
    } printf ("\n");;        

    //ass.code_.instr_[0];
    //Lexer lex = {};
    //lex_init (&lex, src);
//
    //Token tok = {};
    //while ((tok = lex_get_tok (&lex)).type != FINAL) {
    //    if (tok.type == INCORRECT) {
    //        lex_skip_line (&lex);
    //        continue;
    //    }
    //    printf ("%d\'%.*s\':%zu\n", tok.type,
    //        (int)tok.name.size_, tok.name.beg_, tok.nline);
    //}
    //printf ("last:%d\'%.*s\':%zu\n", tok.type,
    //        (int)tok.name.size_, tok.name.beg_, tok.nline);    

    asm_free (&ass);
    if (get_nerrors() == 0) {
        printf ("finished successully\n");
    } else {
        printf ("finished filure\n");
    }
    return 0;
}

#endif // VALUE_H_INCLUDED