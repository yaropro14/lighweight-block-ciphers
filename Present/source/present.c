#include "../include/present.h"

void present_set_key(struct present_t p, int64_t key[2]) {}

int64_t present_encode(struct present_t p, int64_t block) {
    return block;
}

int64_t present_decode(struct present_t p, int64_t block) {
    return block;
}
