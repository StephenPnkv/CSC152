/*
	
	| Stephen Penkov | CSC152-02 | Homework #2 - P1 | 9/30/18 |

	P1: This function implements the pseudocode below. It takes two unsigned
	integers and performs Galois Field multiplication in GF(2^degree).

	Let p1 and p2 be elements of GF(2^n) (ie, polynomials of degree less than n)
	extra = 0
	Loop invariant: answer to original multiplication = (p1)(p2)+extra
	while (p1 not 0)
		if (p1 has x^0 term)
			p1 = p1 - x^0
			extra = extra + p2
		p1 = p1 / x
		p2 = p2 * x
		if (p2 has x^n term)
			p2 = p2 - x^n + (modulus’s lower terms)
	Loop invariant: answer to original multiplication = (p1)(p2)+extra
*/

unsigned mult(unsigned a, unsigned b, unsigned modulus, unsigned degree) {
	
	
	//Loop invariant: answer to original multiplication = (p1)(p2)+extra
	unsigned extra = 0;
	while(a != 0){
		//If a is odd
		if(a & 1){
			//Subtract 1, add b to extra.
			a ^= 1;
			extra ^= b;
		}
		//Divide and multiply by x
		a >>= 1;
		b <<= 1;
		//If b has x^degree term, reduce using the
		//an irredubile polynomial.
		if(b & (1 << degree)){
			//Reduce using modulus.
			b ^= modulus;
		}
	}
	return extra;
}




