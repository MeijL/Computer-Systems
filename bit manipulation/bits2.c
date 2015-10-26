/* 
 * <Bryan Wade>

 */

#if 0
\
INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.



#endif
/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
	
	/*Flips bits of both variables and ORs them to get NAND operation
 	* and then flips the NAND operation.
 	* */
	int z; 
	x = ~x;
	y = ~y;
	z = x | y;
	return ~z;


}
/* 
 * TMax - return maximum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmax(void) {
/*Tmax is 0 followed by 31 1s.  This function creates it by
 * leftshifting a 1 to the very left side of the number and
 * then flipping the bits
 * */
int x = 1;
x = x << 31;

  return ~x;

}
/* 
 * copyLSB - set all bits of result to least significant bit of x
 *   Example: copyLSB(5) = 0xFFFFFFFF, copyLSB(6) = 0x00000000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int copyLSB(int x) {
/*This function first left shifts by 31, which gets rid of all but
 * the least significant bit and puts the least significant bit in
 * the left most position.  It then right shifts it back to its
 * previous position, such that arithmetic shift causes the rest of
 * the bits to be the same as the former LSB.
 * */
x = x << 31;
x = x >> 31;
  return x;
}
/* 
 * leastBitPos - return a mask that marks the position of the
 *               least significant 1 bit. If x == 0, return 0
 *   Example: leastBitPos(96) = 0x20
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2 
 */
int leastBitPos(int x) {
  /* The only location where a number and its two's complement share
 * a bit position is at the least bit position.  Thus, this function
 * ands the number and its two's complement to return a mask of the
 * least bit position.
 * */
return x & (1 + ~x );
}
/* 
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int divpwr2(int x, int n) {
    	int signBit = x >> 31;
	int twoToN = 1 << n;
	
	
	/*detects if x is negative and then corrects for offset if negative.*/
	x = ((signBit) & ((twoToN) + ~0)) + x;
	
	/*divides the number by 2^n*/
	return x >> n;
	
}
/* 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 */
int logicalShift(int x, int n) {
/* This function uses the copy property of arithmetic shift to first
 * get a mask of the locations of where 0s would be placed in logical
 * shift.  It then does an arithmetic shift on the number, but places
 * 0s where 1s might have been copied to, using the mask.
 * */

/* Create zero mask*/
int zeros = 1 << 31;
zeros = zeros >> (n);
zeros = zeros << 1;
zeros = ~zeros;  

/*normal arithmetic shift*/
x = x >> n;

/*correct for 1s with zero mask*/
 return x & zeros;
}
/* 
 * isNonNegative - return 1 if x >= 0, return 0 otherwise 
 *   Example: isNonNegative(-1) = 0.  isNonNegative(0) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 3
 */
int isNonNegative(int x) {
/*Copies the sign bit over to all bit positions via arithmetic
 * shift.  Uses the ! operator to check if its positive or negative.
 * */	
x = x >> 31;
 
 return !x;
}
/* 
 * isGreater - if x > y  then return 1, else return 0 
 *   Example: isGreater(4,5) = 0, isGreater(5,4) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isGreater(int x, int y) {
        int isSignDif = 0;
        int dif = 0;
        int isSame = 0;
        int signOfX = (x >> 31);
        /*Checks if the signs of x and y are different*/
        isSignDif = (signOfX ^ (y >> 31)) &1;
        /*subtracts y from x*/
        dif = x + ~y + 1;
        /*checks if x and y are the same number*/
        isSame = (!!(x ^ y));

	/*ifthe signs of x and y are different x is positive then returns one.  Otherwise
 	* it takes the subtracted result of x and y, and if the sign changes then y is 
 	* greater than x.  Also checks if the numbers are the same.
 	* */
        return ((isSignDif & (~signOfX )) | (~isSignDif & (~dif >> 31))) & isSame ;



 
}
/* 
 * absVal - absolute value of x
 *   Example: absVal(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int absVal(int x) {
	/* This gets the sign bit of the number.  If the sign is negative it
 	 * returns the two's complement whereas if it is positive it returns the
 	 * number.  
 	 * */
	int sign = x >> 31;
	int negation = ~x + 1;
	return (sign & negation) | (~sign & x);
}
/*
 * isPower2 - returns 1 if x is a power of 2, and 0 otherwise
 *   Examples: isPower2(5) = 0, isPower2(8) = 1, isPower2(0) = 0
 *   Note that no negative number is a power of 2.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int isPower2(int x) {
	/*This function check if there is a sigular one in all the bit positions
 	* as if this is the case then the number is a multiple of 2, minus edge
 	* cases.  It then checks for the edge cases 1 and tmin.
 	* */

	/*checks if there is a singular 1 by removing least significant bit 
 	* and seeing if anything is left.
 	* */
	int leastSigPos = x & (~x + 1); 
	int leastBitGone = (x + ~leastSigPos + 1);
 	
	/*checks for edge cases*/
	int isOne = !(x ^(0x1));
	int isMin = x ^ (1<<31);
	
return (!leastBitGone & !!isMin & !!x) |isOne;
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
/* This counts the number of bits in a number.  It does so by
 * first creating the mask 0x11111111.  It ands this mask with
 * x to get 1 digit for every 4 bits in x, and adds this to a 
 * counter.  It then shifts x and repeats the process until all
 * bits are counted in x.  The counter then is changed so that
 * the bits are counted in 1 place as opposed to split up into
 * 4 bit sized locations.
 * */

/*Create bit mask*/
int returnNum = 0;
int bitMask = 0x11;  

bitMask = bitMask + (bitMask << 8);
bitMask = bitMask + (bitMask << 16); 

/*count up bits in x, but put them in seperated parts of the return num*/
returnNum = (bitMask & x) + (bitMask & (x >>1)) + (bitMask & (x>>2)) + (bitMask & (x >>3));

/*fix the return num so that all the bit counts are in 1 place*/
returnNum = returnNum + (returnNum >> 16);
returnNum = (returnNum & 0x0F) + ((returnNum >> 4) & 0x0F) + ((returnNum >> 8) & 0x0F) + ((returnNum >> 12) & 0x0F);


  return returnNum;
}
/* 
 * float_abs - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument..
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_abs(unsigned uf) {
  	
	/*This function first checks to see if the number is NaN.  If so, it
 	* returns the number unchanged.  Otherwise it changes the signbit to
 	* 0, making the number positive, and returns that.
	* */
	int fracTest = ~(0xff800000); 
	int nan = 0x7f800000;
	int nanT = 0xff800000;
	
	
	
	
	if(fracTest & uf)
	{	
		if((nan & uf) == nan)
		{	
			return uf;
		}
		if ((nanT & uf) == nanT)
		{	
			return uf;
		}
	}
	
	return (~(1<< 31) & uf) ;
}
/* 
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x) {
  	int bias = 126;
        int z;
        int exp = 0;
        int frac = 0;
        int xCount = 0;
        int deleBit = 0;
        int numToDel = 0;
        int toAdd = 0;

        /*gets sign of x*/
        int sign = 0x80000000 & x;
        /*checks edge cases*/
        if(x == 0)
        {
                return 0;
        }
        if(x == 0x80000000)
        {
                return 0xCF000000;

        }

        /*absolute value*/
        if(sign)
        {
                x = -x;
       }


        z = x;
  /*counts exponents*/
        while(z)
        {
                z = z >> 1;
                exp = exp + 1;
        }
        xCount = exp;

        exp = exp + bias;
         /*gets rid of leading 1*/
        x = x - (1 << xCount - 1);

        /*checks if rounding is needed*/
        if(xCount > 23)
        {

                numToDel = xCount - 24;
                /*first truncates*/
                toAdd = x >> numToDel;

                /*checks if truncated half is >= 1/2*/
                if(1 & (x >> (numToDel - 1)))
                {
                        /*fixes edge case*/
                        deleBit = x << 1;

                        /*checks if truncated half is greater than 1/2*/
                        deleBit = deleBit << (32 - numToDel);
                        if(deleBit)
                        {
                                toAdd++;

                        }
                        else
                        {
 			/*rounds to even*/
                                if(1 & (toAdd))
                                {
                                        toAdd++;
                                }

                        }
                }

                x = toAdd;
                xCount = 24;
        }

        /*puts the fraction part in the right location and pads with 0s*/
        frac = x;
        frac = frac << (23 - (xCount - 1));

        /*puts exponential part in the right place*/
         exp = exp << 23;

        /*combines the different parts*/
        return sign + frac + exp;

}
