#ifndef CANARY_H_INCLUDED
#define CANARY_H_INCLUDED

#include <stdint.h>

typedef uint64_t canary_t; 

void set_canary (canary_t *pcanary);
bool is_valid_canary (const canary_t *pcanary);

#endif