#include "../include/present.h"
#include <string.h>
/*
 *  read_bit return bit of @word
 */
#define read_bit(word, num) (((word) >> (num)) & 1)

/*
 * add_round_key effect block with one of 32 keys
 * @block - int64_t start stat
 * @key - int64_t key for this round
 */
#define add_round_key(block, key) ((block) ^ (key))

/*
 * s_box_layes change every 16 bit of @block according to table in @p
 */
inline int64_t s_box_layer(struct present_t* p, int64_t block) {
    int64_t bit_mask = 0xf;
    for(int16_t i = 0; i < 0x10; ++i) {
        int64_t bit4word = 0x0;
        
        // use bit_mask to read 16bit
        bit4word = block & bit_mask; 
        bit4word >>= 4*i;
        bit4word = p -> s_block_layer_table[bit4word];
        bit4word <<= 4*i;

        // put 0x0 in place of bit_mask
        block &= (~bit_mask); 
      
        // unit bit4word and block
        block |= bit4word;

        bit_mask <<= 4; 
    }
    return block;
}

inline int64_t p_layer(int64_t word) {
    int64_t res_word = 0x0;
    int64_t shift = 0;
    for(int16_t i = 0; i < 0x40; ++i) {
        res_word |= read_bit(word, 16 * i + shift);
        res_word <<= 1;
        if(i % 4 == 0 && i != 0) {
            shift++;
        }
    }
    return res_word;
}

void key_update(struct present_t* p, int64_t round_counter) {
    int64_t key_main_copy = p -> key_main_part;
    // 61 shift left 
    p -> key_main_part <<= 16;
    p -> key_main_part |= p -> key_change_part;
    p -> key_main_part <<= 45;
    p -> key_main_part |= key_main_copy >> (3 + 16);
    p -> key_change_part = key_main_copy >> 3 & 0xffff;
    
    // modefy 4 first bit
    int64_t bit4word = p -> key_main_part >> 60;
    bit4word = p -> s_block_layer_table[bit4word];
    bit4word <<= 60;
    int64_t bit4map = 0xffff;
    bit4map <<= 60;
    p -> key_main_part &= (~bit4map);
    p -> key_main_part |= bit4word;

    // modefy 19 - 15 bits
    bit4word = p -> key_main_part & 0xf;
    bit4word ^= (round_counter >> 1);
    p -> key_main_part &= (~0xf);
    p -> key_main_part |= bit4word;
    
    bit4word = p -> key_change_part >> 15;
    bit4word ^= round_counter % 2;
    p -> key_change_part &= (~0x8000);
    p -> key_change_part |= bit4word;
}

/*---------------------INTERFASE EMPLEMENTATION-------------------------*/
void present_set_key(struct present_t* p, int64_t key_main_part, int16_t key_change_part) {
    p -> key_main_part = key_main_part;
    p -> key_change_part = key_change_part;

    p -> key_main_part_copy = key_main_part;
    p -> key_change_part_copy = key_change_part;

    p -> s_block_layer_table[0x0] = 0xc;
    p -> s_block_layer_table[0x1] = 0x5;
    p -> s_block_layer_table[0x2] = 0x6;
    p -> s_block_layer_table[0x3] = 0xb;

    p -> s_block_layer_table[0x4] = 0x9;
    p -> s_block_layer_table[0x5] = 0x0;
    p -> s_block_layer_table[0x6] = 0xa;
    p -> s_block_layer_table[0x7] = 0xd;
    
    p -> s_block_layer_table[0x8] = 0x3;
    p -> s_block_layer_table[0x9] = 0xe;
    p -> s_block_layer_table[0xa] = 0xf;
    p -> s_block_layer_table[0xb] = 0x8;
    
    p -> s_block_layer_table[0xc] = 0x4;
    p -> s_block_layer_table[0xd] = 0x7;
    p -> s_block_layer_table[0xe] = 0x1;
    p -> s_block_layer_table[0xf] = 0x2;
}

int64_t present_encode(struct present_t* p, int64_t block) {
    for(int i = 0; i < 31; ++i) {
        block = add_round_key(p, block);
        s_box_layer(p, block);
        p_layer(block);

        key_update(p, i + 1);
    }
    p -> key_main_part = p -> key_main_part_copy;
    p -> key_change_part = p -> key_change_part_copy;
    return block;
}

int64_t present_decode(struct present_t* p, int64_t block) {
    return present_encode(p, block);
}
