#include "Canary.h"
#include "../Utilities/Log/log.h"

void set_canary (canary_t *pcanary)
{
    LOG_MSG (LOG, "Setting canary at %p", pcanary);
    if (pcanary) *pcanary = (canary_t)pcanary;
}

bool is_valid_canary (const canary_t *pcanary)
{
    return pcanary && *pcanary == (canary_t)pcanary;
}