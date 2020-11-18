#include "../include/katan.h"
#include <string.h>

/*
 *  read_bit return bit of @word
 */
#define read_bit(word, num) (((word) >> (num)) & 1)

/*---------------------INTERFASE EMPLEMENTATION-------------------------*/
void katan_set_key(struct katan64_t* p, int64_t key_main_part, int16_t key_change_part) {
    p -> key_main_part = key_main_part;
    p -> key_change_part = key_change_part;
}

/*
 * helps read key bit
 */
int8_t __read_key_bit(struct katan64_t* p, int i) {
    if(i < 16) {
        return (p -> key_change_part >> i) & 1;
    } else {
        return (p -> key_main_part >> (i - 16)) & 1;
    }
}

/*
 * ki return bit of 80b key with i from 0 to 253
 */
int8_t ki(struct katan64_t* p, int i) {
    if(i < 80) {
        return __read_key_bit(p, i);
    } else {
        return __read_key_bit(p, i - 80) ^\
               __read_key_bit(p, i - 61) ^\
               __read_key_bit(p, i - 50) ^\
               __read_key_bit(p, i - 13);
    } 
}

void LFSP_start(struct katan64_t* p) {
    p -> LFSR = 1;
    p -> IR = 1;
}

void LFSR_next(struct katan64_t* p) {
    uint8_t LFSR = p -> LFSR;
    int8_t new_bit = read_bit(LFSR, 7) ^\
                     read_bit(LFSR, 6) ^\
                     read_bit(LFSR, 4) ^\
                     read_bit(LFSR, 2);
    new_bit <<= 7;
    LFSR >>= 1;
    LFSR |= new_bit;

    p -> IR = LFSR & 1;
    p -> LFSR = LFSR;
}

int64_t katan_encode(struct katan64_t* p, int64_t block) {

}

int64_t katan_decode(struct katan64_t* p, int64_t block) {
    return katan_encode(p, block);
}
