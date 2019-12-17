/*
	| Stephen Penkov | CSC152-02 | Homework #4 - P3 | 10/27/18 |
	P3: This is an implementation of polynomial hashing using
	Horner's method and divisionless mod.
*/
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <inttypes.h>


uint64_t poly61(unsigned char m[], unsigned mbytes, uint64_t k){
	
	unsigned __int128 tmp;
	uint64_t lo, hi, res;
	unsigned char rem_bytes[4] = {0};
	res = 1;
	int i = 0;

	while(mbytes - i >= 4){
		//K^n + x0k^n-1 ... xn-1 k
		//Store computation in 128 bit intermediate result
		tmp = ((unsigned __int128)res * k);
		tmp = tmp + *(unsigned *)(&m[i]);
		//Isolate the bits, store in result.
		hi = tmp >> 61;
		lo = tmp & ((1ull << 61) - 1); //divisionless mod
		res = lo + hi;
		i+=4;
	}
	
	//Pad with 10*, process remaining bytes
	memcpy(rem_bytes,&m[i],mbytes - i);
	rem_bytes[mbytes - i] = 0x80;
	tmp = (unsigned __int128)res * k;
	tmp = tmp + *(unsigned*)(&rem_bytes[0]);
	hi = tmp >> 61;
	lo = tmp & ((1ull << 61) - 1);
	res = lo + hi;
	
	int64_t t = res - ((1ull << 61) - 1);
	int64_t msk = t >> 61;
	
	return (msk & res) | (~msk & t);
}









