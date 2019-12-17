/*
	| Stephen Penkov | CSC152-02 | Homework #4 - P1 | 10/27/18 |
	
	P1: This is an implementation of a hash function using the Sponge
	Construction.

*/
#include <stdio.h>
#include <string.h>
#include <math.h>



void P52(unsigned s[12]);

void P52_hash(unsigned char m[], unsigned mbytes, unsigned char res[], unsigned rbytes){
	/*
		Our sponge will have capacity 32 bytes, rate 16 bytes, and a specifiable number of output bytes. 
		Hash inputs should use 10∗1 padding. The code you submit should have the following function defined.
		void P52_hash(unsigned char m[], unsigned mbytes, unsigned char res[], unsigned rbytes)
		It reads mbytes bytes from m and writes the rbytes bytes result to res.
	*/
	const int C_SIZE = 32, R_SIZE = 16; //Size of capacity and rate
	unsigned char blk[48] = {0};
	
	
	//Loop through m, xor with block, call 
	//P52 when 16 bytes are read.
	int i = 0; 
	while(mbytes - i >= R_SIZE){

		for(int j = 0; j < R_SIZE; ++j)
			blk[j] ^= m[j + i];
		
		P52((unsigned*)blk);
		i += R_SIZE;

	}

	//Read remaining bytes
	int j = 0;
	while(i < mbytes){

		blk[j] ^= m[i];
		++j;
		++i;

	}
	//Pad with 10*1
	blk[j] ^= 0x80;
	blk[R_SIZE - 1] ^= 0x01;
	P52((unsigned*)blk);

	//Squeeze
	i = 0; j = 0;
	while(i < rbytes){

		if(j % R_SIZE == 0 && j > 0){
			P52((unsigned*)blk);
			j = 0;
		}
		memcpy(&res[i],&blk[j],1);
		++i;
		++j;

	}
	
}


