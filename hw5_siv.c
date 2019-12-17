/*
	| Stephen Penkov | CSC152-02 | Homework #5 - P1 | 11/26/18 |
	P1: This program implements authenticated encryption using SIV mode.
*/
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#define BLK_LEN 48
#define NONCE_LEN 16
#define IV_LEN 16


void P52_BC(unsigned kbytes, unsigned char key[kbytes],     // 16 or 32 only
            unsigned char in[48], unsigned char out[48]);
void P52_hash(unsigned char m[], unsigned mbytes, unsigned char res[], unsigned rbytes);
uint64_t poly61(unsigned char m[], unsigned mbytes, uint64_t k);
void P52_CTR(unsigned kbytes, unsigned char key[kbytes],  // Upto 48 bytes
             unsigned ivbytes, unsigned char iv[ivbytes], // Upto 48 bytes
             unsigned inbytes, unsigned char in[inbytes], // in == out is okay
             unsigned char out[inbytes]);
int pcm_siv_decrypt(unsigned kbytes, unsigned char k[kbytes],
                    unsigned nbytes, unsigned char n[nbytes],
                    unsigned cbytes, unsigned char c[cbytes],
                    unsigned char p[cbytes], unsigned char t[16]);

void pcm_siv_encrypt(unsigned kbytes, unsigned char k[kbytes], 
					unsigned nbytes, unsigned char n[nbytes], 
					unsigned pbytes, unsigned char p[pbytes],
					unsigned char c[pbytes], unsigned char t[16])
{

	unsigned char blk[BLK_LEN] = {0},
				  keys[BLK_LEN] = {0}, 
				  k_pad[BLK_LEN] = {0};

	memset(blk,0xFF,BLK_LEN);
	memcpy(k_pad,k,kbytes);
	
	P52_BC(kbytes, k_pad,blk,keys);

	uint64_t hash_key1;
	hash_key1 = *(uint64_t*)(&keys[0]);
	uint64_t hash_key2;
	hash_key2 = *(uint64_t*)(&keys[8]);
	
	hash_key1 >>= 4;
	hash_key2 >>= 4;

	uint64_t hash1 = poly61(p,pbytes,hash_key1);
	uint64_t hash2 = poly61(p,pbytes,hash_key2);

	//keys = siv = P52-BC(k, hash1 || hash2 || nonce || 10*)
	memset(blk,0x00,BLK_LEN);
	memset(keys,0x00,BLK_LEN);

	unsigned char buf[8];
	int j = 0;

	memcpy(buf,&hash1,8);
	for(int i = 0; i < 8; ++i){
		blk[j] = buf[i];
		++j;
	}
	
	memcpy(buf,&hash2,8);
	for(int i = 0; i < 8; ++i){
		blk[j] = buf[i];
		++j;
	}
	for(int i = 0; i < nbytes; ++i){
		blk[j] = n[i];
		++j;
	}
	
	blk[nbytes + 16] = 0x80;
	
	P52_BC(kbytes, k_pad,blk,keys);

	unsigned char iv[IV_LEN];
	memcpy(iv,keys,IV_LEN);
	//c = P52-CTR(k, siv[0..15], p)
	P52_CTR(kbytes,k,IV_LEN,iv,pbytes,p,c);
	//t = siv[0..15]
	memcpy(t,iv,IV_LEN);
}


