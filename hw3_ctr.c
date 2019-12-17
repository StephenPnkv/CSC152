/*
 * _________________@file hw3_ctr.c______________________
 * | Stephen Penkov | CSC152-02 | Homework #3 - P2      |
 * ------------------------------------------------------
 * | 10/13/18                                           |
 * ------------------------------------------------------
 * | P2: Suppose to encrypt or decrypt stdin to stdout. |
 * ------------------------------------------------------
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NONCE_LEN 12
#define BLK_LEN 48
#define KEY_LEN 48


void P52(unsigned s[12]);
//Function that xors two blocks.
void P52_BC(unsigned char *x, unsigned char counter[BLK_LEN], unsigned char key[KEY_LEN]){
    
    for (int i = 0; i < BLK_LEN; ++i)
    {
        x[i] = counter[i] ^ key[i];
    }
}

int main(int argc, const char* argv[]) {
    size_t bytes_read;
    unsigned char key[KEY_LEN] = {0},  /* Auto init to all zeros */
    nonce[NONCE_LEN] = {0},
    blk[BLK_LEN] = {0},
    counter[BLK_LEN] = {0},
    new_blk[BLK_LEN] = {0};
    

    if ((argc == 3) && (strlen(argv[1]) == 1) && (strlen(argv[2]) <= KEY_LEN) &&
                    ((argv[1][0] == 'e') || (argv[1][0] == 'd'))) {
        if (argv[1][0] == 'e') {
            FILE *rand = fopen("/dev/urandom", "rb");
            fread(nonce, 1, NONCE_LEN, rand);
            fclose(rand);
            fwrite(nonce, 1, NONCE_LEN, stdout);
        } else {
            fread(nonce, 1, NONCE_LEN, stdin);
        }
        memcpy(key, argv[2], strlen(argv[2]));
    } else {
        fprintf(stderr, "usage: %s d|e <pwd up to 48 chars>\n", argv[0]);
        return EXIT_FAILURE;
    }

    do {
        bytes_read = fread(blk, 1, BLK_LEN, stdin);  
        if (bytes_read > 0) {
            // process bytes_read bytes
            //Read nonce into counter block.
            memcpy(counter,nonce,NONCE_LEN);
            //P52(X xor k) xor k
            P52_BC(new_blk,counter,key);
            P52((unsigned*)new_blk);
            P52_BC(new_blk,new_blk,key);
           
            // Xor block with plaintext or ciphertext
            P52_BC(new_blk,new_blk,blk);
            //Increment counter big endian
            for (int i = BLK_LEN - 1; i >= 0; i--){
                ++counter[i];
                if(counter[i] != 0)
                    break;
            }
            fwrite(new_blk, 1, bytes_read, stdout);


        }
    } while (bytes_read == BLK_LEN);


    return EXIT_SUCCESS;
    
}
















