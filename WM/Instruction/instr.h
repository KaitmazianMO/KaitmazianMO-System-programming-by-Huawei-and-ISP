#ifndef INSTRUCTION_H_INCLUDED
#define INSTRUCTION_H_INCLUDED

#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>
#include "../../Utilities/Strings/StringView.h"

/*                      
 * Isntruction := { cmd , args } 
 *           move rax, 1    add rax, rbx, 5         jmp 4
 * args := { reg, ref/reg | reg, ref/reg, ref/reg | addr }   
 *   
 * if reg/ref 
 */

/* typedefs for args, ref_t can be a reg if first bit is zero(refs begin with 1 in first bit) */ 
typedef uint32_t reg_t;
typedef uint32_t regref_t;
typedef uint32_t ref_t; 
#define REGREF_F "%" PRIu32

#define INSTR_CNTX_REPR( instr, opcode, ... )\
    op_ ## instr = opcode,

enum Opcode {
    #include "../IntructionSetDSL/instr_def.h"
};

static constexpr size_t G_INSTR_NBITS = 32u;
static constexpr size_t G_INSTR_OPCODE_NBITS = 6u;
static constexpr size_t G_INSTR_ARG_NBITS = G_INSTR_NBITS - G_INSTR_OPCODE_NBITS; 
/* min nuber of bits for reg */
static constexpr size_t G_INSTR_REG_NBITS = 6u;
/* bits for reg or ref in binary operations (add rax, A, B) */
static constexpr size_t G_INSTR_AB_NBITS = (G_INSTR_ARG_NBITS - G_INSTR_REG_NBITS) / 2;

/* max sizes for instr args,  */
static constexpr size_t G_INSTR_REG_T_MAX    = (1u << G_INSTR_REG_NBITS);
static constexpr size_t G_INSTR_REGREF_T_MAX = (1u << G_INSTR_AB_NBITS);
static constexpr size_t G_INSTR_REF_T_MAX    = (1u << G_INSTR_ARG_NBITS);

static constexpr size_t G_INSTR_N_REGISTERS  = G_INSTR_REG_T_MAX;

struct Instruction {
    unsigned cmd  : G_INSTR_OPCODE_NBITS;
    unsigned args : G_INSTR_ARG_NBITS;
};

#if ((1u << G_INSTR_OPCODE_NBITS) < op_N)
#error "TOO MANY OPCODES FOR THIS INSTRUCTION FORMAT (G_INSTR_OPCODE_NBITS:"__FILE__")" 
#endif

const char *instr_to_str (Opcode);

struct ReservedCommand {
    Opcode id;
    const char *name;
};

ReservedCommand find_reserved_command (const StringView sv);
reg_t instr_to_reg (regref_t rr);
ref_t instr_to_ref (regref_t rr);

#endif