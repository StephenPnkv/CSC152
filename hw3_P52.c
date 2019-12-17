/*
	| Stephen Penkov | CSC152-02 | Homework #3 - P1 | 10/12/18 |
	
	P1: This is our second implementation of the P52 permutation using
	SSE2 registers and the Intel Intrinsics instructions. The program utilizes
	parallelism, operating on all 4 bytes in the register at the same time. The
	contents are encrypted in O(n) time.

*/
#include <emmintrin.h>

void P52(unsigned s[12]){
	
	//6 SSE2 registers used.
	__m128i r1,r2,r3,s1,s2,s3;
	
	for(int i = 0; i < 24; ++i){
		
		//X,Y,Z -> registers r1,r2,r3.
		//Load the bytes into registers and do rotations.
		r1 = _mm_loadu_si128((__m128i*)s);
		r1 = _mm_or_si128 (_mm_slli_epi32(r1,24),_mm_srli_epi32(r1,32 - 24));

		r2 = _mm_loadu_si128((__m128i*)(&s[4]));
		r2 = _mm_or_si128 (_mm_slli_epi32(r2,9),_mm_srli_epi32(r2,32 - 9));
		
		r3 = _mm_loadu_si128((__m128i*)(&s[8]));

		//Core encryption operations using functional programming.
		s3 = _mm_xor_si128(
				_mm_xor_si128(r1,_mm_slli_epi32(r3,1)),
				_mm_slli_epi32(_mm_and_si128(r2,r3),2)
		);
		_mm_storeu_si128((__m128i *)(&s[8]),s3);
		
		s2 = _mm_xor_si128(
				_mm_xor_si128(r2,r1),
				_mm_slli_epi32(_mm_or_si128(r1,r3),1)
		);
		_mm_storeu_si128((__m128i *)(&s[4]),s2);
		
		s1 = _mm_xor_si128(
				_mm_xor_si128(r3,r2),
				_mm_slli_epi32(_mm_and_si128(r1,r2),3)
		);
		_mm_storeu_si128((__m128i *)s,s1);

		//Shuffle the contents of the first row to add diffusion.
		if(i % 4 == 0){
			s1 = _mm_shuffle_epi32(s1, _MM_SHUFFLE(2,3,0,1));
			_mm_storeu_si128((__m128i *)s,s1);
			s[0] ^= (0x9e377900 ^ (24 - i));
			
		}else if(i % 4 == 2){
			s1 = _mm_shuffle_epi32(s1, _MM_SHUFFLE(1,0,3,2));
			_mm_storeu_si128((__m128i *)s,s1);
		}
	}	
}


