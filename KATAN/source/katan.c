#include "../include/katan.h"
#include <string.h>

/*
 *  read_bit return bit of @word
 */
#define read_bit(word, num) (((word) >> (num)) & 1)

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
 * ki return bit of 80b key with i from 0 to 253
 */
uint8_t ki(struct katan64_t* p, uint32_t i) {
    if(i < 80) {
        return __read_key_bit(p, i);
    } else {
        return __read_key_bit(p, i - 80) ^\
               __read_key_bit(p, i - 61) ^\
               __read_key_bit(p, i - 50) ^\
               __read_key_bit(p, i - 13);
    } 
}

void LFSR_start(struct katan64_t* p) {
    p -> LFSR = 1;
    p -> IR = 1;
}

void LFSR_next(struct katan64_t* p) {
    uint8_t LFSR = p -> LFSR;
    uint8_t new_bit = read_bit(LFSR, 7) ^\
                     read_bit(LFSR, 6) ^\
                     read_bit(LFSR, 4) ^\
                     read_bit(LFSR, 2);
    new_bit <<= 7;
    LFSR >>= 1;
    LFSR |= new_bit;

    p -> IR = LFSR & 1;
    p -> LFSR = LFSR;
}

uint64_t katan_encode(struct katan64_t* p, uint64_t block) {
    LFSR_start(p);
    uint64_t L1 = (block >> p -> L2_len);
    uint64_t L2 = (block << p -> L1_len) >> p -> L1_len;
    
    for(uint16_t i = 0; i < 254; ++i) {
        uint8_t ka = ki(p, 2*i);
        uint8_t kb = ki(p, 2*i + 1);

        uint64_t L1_update = read_bit(L1, p -> x1) ^\
                            read_bit(L1, p -> x2) ^\
                            (read_bit(L1, p -> x3) & read_bit(L1, p -> x4)) ^\
                            (read_bit(L1, p -> x5) & p -> IR) ^ ka;
        uint64_t L2_update = read_bit(L2, p -> y1) ^\
                            read_bit(L2, p -> y2) ^\
                            (read_bit(L2, p -> y3) & read_bit(L1, p -> y4)) ^\
                            (read_bit(L2, p -> y5) & read_bit(L1, p -> y6)) ^ kb;
        
        L2 <<= 1;
        L2 |= L2_update;

        L1 >>= 1;
        L1_update <<= (p -> L1_len - 1);
        L1 |= L1_update;

        LFSR_next(p);
    }
    
    block = 0;
    block |= L1;
    block <<= p -> L1_len;
    block |= L2;
    
    return block;
}

uint64_t katan_decode(struct katan64_t* p, uint64_t block) {
    return katan_encode(p, block);
}
