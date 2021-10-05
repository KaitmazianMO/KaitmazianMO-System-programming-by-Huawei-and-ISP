#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED

#include <sys/types.h>
#include <inttypes.h>
#include <stdint.h>

typedef uint64_t hash_t;
#define HASH_FMT  "%" PRId64
hash_t compute_hash (const char *mem, size_t mem_size, size_t mod = (hash_t)-1);

#endif