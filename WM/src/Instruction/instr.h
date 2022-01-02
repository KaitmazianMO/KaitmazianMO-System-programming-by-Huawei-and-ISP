#ifndef INSTRUCTION_H_INCLUDED
#define INSTRUCTION_H_INCLUDED

#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>
#include "../../Utilities/Strings/StringView.h"

/*                      
 * Isntruction := { cmd , args } 
 *           move R1 1    add R1 R2 5         jmp 4
 * args := { reg, ref/reg | reg, ref/reg, ref/reg | addr }   
 *   
 * if reg/ref 
 */

/* typedefs for args, ref_t can be a reg if it less than G_INSTR_N_REGS */ 
typedef uint32_t arg_t;
#define REGREF_F "%" PRIu32

#define INSTR_CNTX_REPR( instr, opcode, ... )\
    op_ ## instr = opcode,

enum Opcode {
    #include "../IntructionSetDSL/instr_def.h"
};

static constexpr size_t G_INSTR_NBITS        = 32u;
static constexpr size_t G_INSTR_OPCODE_NBITS = 6u;
static constexpr size_t G_INSTR_ARG_NBITS    = G_INSTR_NBITS - G_INSTR_OPCODE_NBITS; 

static constexpr size_t G_INSTR_ARG_R_NBITS = 6u;
static constexpr size_t G_INSTR_ARG_A_NBITS = (G_INSTR_ARG_NBITS - G_INSTR_ARG_R_NBITS) / 2;
static constexpr size_t G_INSTR_ARG_B_NBITS = (G_INSTR_ARG_NBITS - G_INSTR_ARG_R_NBITS - G_INSTR_ARG_A_NBITS);
static constexpr size_t G_INSTR_ARG_C_NBITS = (G_INSTR_ARG_NBITS - G_INSTR_ARG_R_NBITS);
static constexpr size_t G_INSTR_ARG_D_NBITS = (G_INSTR_ARG_NBITS);

/* max sizes for instr args,  */
static constexpr size_t G_INSTR_ARG_R_MAX = (1u << G_INSTR_ARG_R_NBITS);
static constexpr size_t G_INSTR_ARG_A_MAX = (1u << G_INSTR_ARG_A_NBITS);
static constexpr size_t G_INSTR_ARG_B_MAX = (1u << G_INSTR_ARG_B_NBITS);
static constexpr size_t G_INSTR_ARG_C_MAX = (1u << G_INSTR_ARG_C_NBITS);
static constexpr size_t G_INSTR_ARG_D_MAX = (1u << G_INSTR_ARG_D_NBITS);

static constexpr size_t G_INSTR_N_REGISTERS  = G_INSTR_ARG_R_MAX;

struct Instruction_RAB {
    unsigned opcode : G_INSTR_OPCODE_NBITS;
    unsigned R      : G_INSTR_ARG_R_NBITS;
    unsigned A      : G_INSTR_ARG_A_NBITS;
    unsigned B      : G_INSTR_ARG_B_NBITS; 
};

struct Instruction_RC {
    unsigned opcode : G_INSTR_ARG_R_NBITS;
    unsigned R      : G_INSTR_ARG_R_NBITS;
    unsigned C      : G_INSTR_ARG_C_NBITS;
};

struct Instruction_D {
    unsigned opcode : G_INSTR_OPCODE_NBITS;
    unsigned D      : G_INSTR_ARG_D_NBITS;
};

struct Instruction {
    union {
        Instruction_RAB RAB;
        Instruction_RC  RC;  
        Instruction_D   D;
    };
};

static_assert (sizeof (Instruction_RAB) == G_INSTR_NBITS / 8 && "Instuction size verification failed");
static_assert (sizeof (Instruction_RC)  == G_INSTR_NBITS / 8 && "Instuction size verification failed");
static_assert (sizeof (Instruction_D)   == G_INSTR_NBITS / 8 && "Instuction size verification failed");
static_assert (sizeof (Instruction)     == G_INSTR_NBITS / 8 && "Instuction size verification failed");

static_assert (op_N <= 1u << G_INSTR_OPCODE_NBITS && "Too many opcodes for this instruction format, check G_INSTR_OPCODE_NBITS");

const char *instr_to_str (Opcode);

struct ReservedCommand {
    Opcode id;
    const char *name;
};

ReservedCommand find_reserved_command (const StringView sv);

static constexpr arg_t G_INSTR_ARG_INVALID = (arg_t)(-1);

// A, B, C, D may be regs, so use instr_to_R in reg case
arg_t instr_to_R (arg_t arg); // R as reg
arg_t instr_to_A (arg_t arg); // A as ref
arg_t instr_to_B (arg_t arg); // B as ref
arg_t instr_to_C (arg_t arg); // C as ref
arg_t instr_to_D (arg_t arg); // D as ref

#endif