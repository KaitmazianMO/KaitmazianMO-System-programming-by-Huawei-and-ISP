#include "hash.h"
#include "log.h"

LOG_WIHTOUT_TRACE
static char roll (char);

hash_t compute_hash (const char *mem, size_t mem_size, size_t mod)
{
    hash_t hash = (hash_t)-1 % mod;
    char *phash = (char *)&hash;
    for (size_t i = 0; i < mem_size; ++i)
        phash[i%sizeof(hash_t)] ^= roll (mem[i]);

    logger_message (LOG, "Computed hash: %zu", hash % mod);
    return hash % mod;
}

char roll (char ch)
{
    return (ch << 1) | (ch >> 7);
}