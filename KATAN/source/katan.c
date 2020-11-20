#include "../include/katan.h"
#include <string.h>
#include <stdio.h>

/*
 *  read_bit return bit of @word
 */
#define read_bit(word, num) (((word) >> (num)) & 1)

void __bit_print(uint64_t b, int i) {
    if(i == 0) {
        return;
    } else {
        __bit_print(b / 2, i - 1);
        printf("%lu", b % 2);
    }
}

void bit_print(uint64_t b) {
    __bit_print(b, 64);
    putchar('\n');
}

/*
 * helps read key bit
 */
uint8_t __read_key_bit(struct katan64_t* p, uint32_t i) {
    if(i < 16) {
        return read_bit(p -> key_change_part, i);
    } else {
        return read_bit(p -> key_main_part, i - 16);
    }
}

/*
 * ki return bit of 80b key with i from 0 to 511
 */
uint8_t ki(struct katan64_t* p, uint32_t i) {
    if(i < 80) {
        return __read_key_bit(p, i);
    } else {
        return ki(p, i - 80) ^\
               ki(p, i - 61) ^\
               ki(p, i - 50) ^\
               ki(p, i - 13);
    } 
}

void LFSR_start(struct katan64_t* p) {
    p -> LFSR = 0xff;
}

void LFSR_next(struct katan64_t* p) {
    uint8_t LFSR = p -> LFSR;
    uint8_t new_bit = read_bit(LFSR, 7) ^\
                     read_bit(LFSR, 6) ^\
                     read_bit(LFSR, 4) ^\
                     read_bit(LFSR, 2);
    LFSR <<= 1;
    LFSR |= new_bit;
    p -> IR = LFSR >> 7;
    p -> LFSR = LFSR;
}

/*---------------------uintERFASE EMPLEMENTATION-------------------------*/
void katan_set_key(struct katan64_t* p, uint64_t key_main_part, uint16_t key_change_part) {
    p -> key_main_part = key_main_part;
    p -> key_change_part = key_change_part;

    p -> L1_len = 25;
    p -> L2_len = 39;
    
    p -> x1 = 24;
    p -> x2 = 15;
    p -> x3 = 20;
    p -> x4 = 11;
    p -> x5 = 9;

    p -> y1 = 38;
    p -> y2 = 25;
    p -> y3 = 33;
    p -> y4 = 21;
    p -> y5 = 14;
    p -> y6 = 9;
}


uint64_t katan_encode(struct katan64_t* p, uint64_t block) {
    LFSR_start(p);
    uint64_t L1 = (block >> p -> L2_len);
    uint64_t L2 = block - (L1 << p -> L2_len); 

    for(uint16_t i = 0; i < 254; ++i) { 
        uint8_t ka = ki(p, 2*i);
        uint8_t kb = ki(p, 2*i + 1);

        // only for KATAN64
        for(uint16_t j = 0; j < 3; ++j) {
            uint64_t a_update = read_bit(L1, p -> x1) ^\
                                 read_bit(L1, p -> x2) ^\
                                 (read_bit(L1, p -> x3) & read_bit(L1, p -> x4)) ^\
                                 (read_bit(L1, p -> x5) & p -> IR) ^ ka;

            uint64_t b_update = read_bit(L2, p -> y1) ^\
                                 read_bit(L2, p -> y2) ^\
                                 (read_bit(L2, p -> y3) & read_bit(L2, p -> y4)) ^\
                                 (read_bit(L2, p -> y5) & read_bit(L2, p -> y6)) ^ kb;
            L1 <<= 1;
            L1 |= b_update;
            L1 <<= p -> L2_len;
            L1 >>= p -> L2_len;

            L2 <<= 1;
            L2 |= a_update;
            L2 <<= p -> L1_len;
            L2 >>= p -> L1_len;
        }
        LFSR_next(p);
    }
    
    block = 0;
    for(int i = 0; i < p -> L2_len; ++i) {
        block |= L2 & 1;
        block <<= 1;
        L2 >>= 1;
    }
    for(int i = 0; i < p -> L1_len; ++i) {
        block |= L1 & 1;
        if(i != p -> L1_len - 1) {
            block <<= 1;
        }
        L1 >>= 1;
    }
    return block;
}

uint64_t katan_decode(struct katan64_t* p, uint64_t block) {
    return katan_encode(p, block);
}

#ifdef DEBUG
void katan_test() {
    uint16_t X = 0xaff;
    uint16_t L1 = (X >> 8);
    uint16_t L2 = X - (L1 << 8);
    printf("%x -> %x %x\n", X, L1, L2);

    printf("1010 == %d%d%d%d\n", read_bit(0xa, 3), read_bit(0xa, 2), read_bit(0xa, 1), read_bit(0xa, 0));

}
#endif
