/* ==========================================
        See random_nums.h for documentation
   ========================================== */

#include "random_nums.h"

// see random_nums.h
float random_real (void) {
    return (float)rand() / (float)RAND_MAX;
}

// see random_nums.h
bool flip (const float prob) {
    float rand = random_real();
    if (rand <= prob) return 1;
    return 0;
}

// see random_nums.h
int random_int (const int low, const int high) {
    return (random_real() * ((high-low)+1)) + low;
}
