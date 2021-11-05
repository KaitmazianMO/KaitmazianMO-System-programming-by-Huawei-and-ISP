#include "hash.h"
#include "../Utilities/Log/log.h"

LOG_WITHOUT_TRACE
static char roll (char ch, int n);

hash_t compute_hash (const char *mem, size_t mem_size, size_t mod)
{
    hash_t hash = 0;
    for (size_t i = 0; i < mem_size; ++i)
        hash += roll (mem[i], i);

    LOG_MSG (LOG, "Computed hash: " HASH_FMT, hash % mod);
    return hash % mod;
}

char roll (char ch, int n)
{
    char val = ch;
    for (int i = 0; i < n % 8; ++i)
        val = (val << 1) | (val >> 7);

    return val;
}