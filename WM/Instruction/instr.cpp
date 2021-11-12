#include "instr.h"
#include "../Error/err.h"
#include <stdlib.h>
#include <string.h>

const char *instr_to_str (Opcode opcode) {
    #define INSTR_CNTX_REPR( name, opcode, ... )\
        #name, 

    static const char *names[op_N+1] = {
        #include "../IntructionSetDSL/instr_def.h"
    };
    names[op_N] = "Unknown";

    if (opcode < op_N) {
        return names[opcode];
    } 
    return names[op_N];
}

int res_cmd_cmp (const void *lvoid, const void *rvoid) {
    const ReservedCommand *l = (const ReservedCommand *)(lvoid);
    const ReservedCommand *r = (const ReservedCommand *)(rvoid);
    return strcmp (l->name,  r->name);
} 

int cmp_sv_cmd (const void *sv_search_v, const void *key_v) {
    const StringView *psv_view = (const StringView *)sv_search_v;
    const ReservedCommand *pkey = (const ReservedCommand *)key_v;

    auto name_length  = strlen (pkey->name);
    auto sv_lenght    = psv_view->size_;
    size_t min_lenght = (sv_lenght < name_length) ? sv_lenght : name_length;
    int cmp = strncmp (psv_view->beg_, pkey->name, min_lenght);
    if (cmp == 0) { /* no extra sym at the end of sv */
        if (min_lenght == sv_lenght) {
            return 0;
        } else { /* cmda > cmd */
            return 1;
        }
    }
    return cmp;
}

ReservedCommand find_reserved_command (const StringView sv) {
    #define INSTR_CNTX_REPR( iname, opcode, ... )\
        { .id = (Opcode)opcode, .name = #iname }, 

    static ReservedCommand cmds[] = {
        #include "../IntructionSetDSL/instr_def.h"
    };

    cmds[op_N].name = "";

    static bool sorted = false;
    if (!sorted) {
        qsort (cmds, op_N, sizeof (cmds[0]), res_cmd_cmp);
        sorted = true;
    }

    ReservedCommand *found = 
        (ReservedCommand *)bsearch ((const void *)&sv, cmds, op_N,
         sizeof (ReservedCommand), cmp_sv_cmd);

    if (found) {
        return *found;
    }

    return {.id = op_N};
}

#define DEFINE_INSTR_TO_( ARG, FORMATED, ... )   \
arg_t instr_to_ ## ARG (arg_t ARG) {    \
    arg_t formated = FORMATED; \
    if (formated >= G_INSTR_ARG_ ## ARG ## _MAX) { \
        error (__VA_ARGS__);  \
        return G_INSTR_ARG_R_MAX;   \
    }   \
    return formated; \
}

#define DEFINE_INSTR_TO_( ARG, FORMAT)  \
    arg_t instr_to_ ## ARG (arg_t ARG) {   \
        if (FORMAT < G_INSTR_ARG_ ## ARG ##_MAX) {    \
            return FORMAT;   \
        }   \
        return G_INSTR_ARG_INVALID; \
    }
    
DEFINE_INSTR_TO_ (R, R)
DEFINE_INSTR_TO_ (A, A + G_INSTR_N_REGISTERS)
DEFINE_INSTR_TO_ (B, B + G_INSTR_N_REGISTERS)
DEFINE_INSTR_TO_ (C, C + G_INSTR_N_REGISTERS)
DEFINE_INSTR_TO_ (C, C + G_INSTR_N_REGISTERS)
DEFINE_INSTR_TO_ (D, D + G_INSTR_N_REGISTERS)
