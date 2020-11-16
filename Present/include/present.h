#include<stdint.h>
/*
 *  present_t - struct that include on part of date that is needed to encode and decode
 */
struct present_t {
    int64_t key[2];   
};

/*
 * present_set_key use 128b key for encode next block
 * @p - struct that contains close part of algorithm. 
 * @key - 128b key, if key is longer then 2 int64_t, other past will be ignored
 */
void present_set_key(struct present_t p, int64_t key[2]);

/*
 * present_encode is main part of implementation, that uses to encode 1 block
 * @p - struct that contains close part of algorithm. 
 * @block - 64b block of open word
 *
 * @return - 64b block of encode word
 */
int64_t present_encode(struct present_t p, int64_t block);

/*
 * present_decode decodes block that generate by present_encode
 * @p - struct that contains close part of algorithm. 
 * @block - 64b block of encode word
 *
 * @return - 64b block of open word
 */
int64_t present_decode(struct present_t p, int64_t block);
