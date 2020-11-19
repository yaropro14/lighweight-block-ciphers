#include<stdio.h>
#include"../include/katan.h"

int main() {
    uint64_t block = 0xface;
    
    katan_test();

    struct katan64_t p;
    katan_set_key(&p, 0xf0f0f0f0f0f0f0f0, 0xf0f0);
    printf("block is %lx\n", block);
    block = katan_encode(&p, block);
    printf("encode block is %lx\n", block);
    block = katan_decode(&p, block);
    printf("decode block is %lx\n", block);
    
    return 0;
}
