/*
	| Stephen Penkov | CSC152-02 | Homework #2 - P2 | 9/30/18 |
	
	P2: This is the implementation of the P52 permutation as given to us by the author. 
	It takes a 12 element array and encrypts the content using XOR magic.

*/



void P52(unsigned s[12]){

	unsigned x = 0, y = 0, z = 0;
	//Loop twice through the array.
	for(int i = 0; i < 24; ++i){

		for(int j = 0; j < 4; ++j){
			
			//Set x,y,z values.
			x = (s[j] << 24) | (s[j] >> (32 - 24));
			y = (s[j + 4] << 9) | (s[j + 4] >> (32 - 9));
			z = s[j + 8];

			//Encryption operations
			s[j + 8] = x ^ (z << 1) ^ ((y & z) << 2);
			s[j + 4] = y ^ x ^ ((x | z) << 1);
			s[j] = z ^ y ^ ((x & y) << 3);
		}
		//Thanks to CSC152, I now know about swapping
		//values of same type using bitwise XOR, 
		//requiring no extra memory!
		if(i % 4 == 0){
			s[0] ^= s[1];
			s[1] ^= s[0];
			s[0] ^= s[1];
			s[2] ^= s[3];
			s[3] ^= s[2];
			s[2] ^= s[3];
			s[0] ^= (0x9e377900 ^ (24 - i));
		}else if(i % 4 == 2){
			s[0] ^= s[2];
			s[2] ^= s[0];
			s[0] ^= s[2];
			s[1] ^= s[3];
			s[3] ^= s[1];
			s[1] ^= s[3];
		}

	}	
}


