#ifndef PRESENT_CODE
#define PRESENT_CODE

#include<stdint.h>
/*
 *  present_t - struct that include on part of date that is needed to encode and decode
 */
struct present_t {
    int64_t key_main_part;
    int16_t key_change_part;

    int64_t key_main_part_copy;
    int16_t key_change_part_copy;

    int8_t s_block_layer_table[16];
};

/*
 * present_set_key use 128b key for encode next block
 * @p - struct that contains close part of algorithm. 
 * @key_main_part - 64b key, that is used in encode
 * @key_chang_part - 16b key, that is used for key update
 */
void present_set_key(struct present_t* p, int64_t key_main_part, int16_t key_change_part);

/*
 * present_encode is main part of implementation, that uses to encode 1 block
 * @p - struct that contains close part of algorithm. 
 * @block - 64b block of open word
 *
 * @return - 64b block of encode word
 */
int64_t present_encode(struct present_t* p, int64_t block);

/*
 * present_decode decodes block that generate by present_encode
 * @p - struct that contains close part of algorithm. 
 * @block - 64b block of encode word
 *
 * @return - 64b block of open word
 */
int64_t present_decode(struct present_t* p, int64_t block);

#endif
