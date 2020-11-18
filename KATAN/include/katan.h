#ifndef katan_CODE
#define katan_CODE

#include<stdint.h>
/*
 *  katan_t - struct that include on part of date that is needed to encode and decode
 */
struct katan64_t {
    int64_t key_main_part;
    int16_t key_change_part;

    int64_t L1, L2;

    int32_t L1_len, L2_len;
    int32_t x1, x2, x3, x4, x5, IR;
    int32_t y1, y2, y3, y4, y5, y6;

    uint8_t LFSR; // uses for IR;
};
/*
 * katan_set_key use 128b key for encode next block
 * @p - struct that contains close part of algorithm. 
 * @key_main_part - 64b key, that is used in encode
 * @key_chang_part - 16b key, that is used for key update
 */
void katan_set_key(struct katan64_t* p, int64_t key_main_part, int16_t key_change_part);

/*
 * katan_encode is main part of implementation, that uses to encode 1 block
 * @p - struct that contains close part of algorithm. 
 * @block - 64b block of open word
 *
 * @return - 64b block of encode word
 */
int64_t katan_encode(struct katan64_t* p, int64_t block);

/*
 * katan_decode decodes block that generate by katan_encode
 * @p - struct that contains close part of algorithm. 
 * @block - 64b block of encode word
 *
 * @return - 64b block of open word
 */
int64_t katan_decode(struct katan64_t* p, int64_t block);

#endif
