/* 
 * <Bryan Wade>
 */

#if 0

INTEGER CODING RULES:
 

 
  int Funct(arg1, arg2, ...) {
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

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use a cast operator.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.



#endif
/*
 * isTmin - returns 1 if x is the minimum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmin(int x) {
/*Adds the int to itself and determines if the new number overflowed to 0.  The only two numbers that do this are Tmin and 0, and the !temp discludes 0*/

int temp = x;
x = x + x;
x = !x ^ (1 & !temp);

  return x;

}

/* 
 * TMax - return maximum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmax(void) {
/*tmax is a 0 followed by a bunch of 1s.  This function takes a one and moves it to the last position and then inverts all the numbers.*/
int x = 1;
x = x << 31;
x = ~x;

return x;


}
/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
	/*This function right shifts x by n - 1 and if x is now all 0s or all 1s then it returns true*/	
	
	x = x >> (n + ~0);
	x = !x | !~x;
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
	/*the only 1 that x and xs complement share is its leastbitpos.  So this 
	* ands the two together and as such returns the position of the one.  
	*/
  	x = x & (~x + 1);
	return x;
}
/* 
 * oddBits - return word with all odd-numbered bits set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 2
 */
int oddBits(void) {
	/*This function takes the binary string 01010101 and copies it to all 4 possible locations
	* by shifting the string and adding it to other possible shifted representations of it.
	*/
  	int a = 0xaa;
	a = a + (a << 8) + (a << 16) + (a << 24);
	return a;
}
/* 
 * sign - return 1 if positive, 0 if zero, and -1 if negative
 *  Examples: sign(130) = 1
 *            sign(-23) = -1
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 10
 *  Rating: 2
 */
int sign(int x) {
	
	/*This checks if the function is 0, and then moves the sign bit to the begining
	* since its an arithmatic shift the 1s copy to all locations if its negative and
	* it because -1 if the sign is negative.  
	 */
   	int isZero = 0;
	
	
	isZero = !!x;
	x = (x >> 31);
	
	 return isZero | x;
}
/* 
 * isGreater - if x > y  then return 1, else return 0 
 *   Example: isGreater(4,5) = 0, isGreater(5,4) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isGreater(int x, int y) {
  	
	int difSign = 0;
	int isDif = 0;
	int isSame = 0;
	int xSign = (x >> 31);
	/*Checks if the signs of x and y are different*/
	difSign = (xSign) ^ (y >> 31);
	/*subtracts y from x*/
	isDif = x + ~y + 1;
	/*checks if x and y are the same number*/
	isSame = (!!(x ^ y)) << 31 >> 31;

	/*ifthe signs of x and y are different x is positive then returns one.  Otherwise
	* it takes the subtracted result of x and y, and if the sign changes then y is 
	* greater than x.  Also checks if the numbers are the same.
	*/
	return ((difSign & (~xSign & 1)) | (~difSign & (~isDif >> 31 & 1))) & isSame ;
}
/* 
 * subOK - Determine if can compute x-y without overflow
 *   Example: subOK(0x80000000,0x80000000) = 1,
 *            subOK(0x80000000,0x70000000) = 0, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int subOK(int x, int y) {
  	int mightFail = 0;
	int newNum = 0;
	int difSign = 0;
	
	/*checks if the signs are different*/
	mightFail = (x>>31) ^ (y >> 31);
	/*adds x and -y*/
	newNum = x + ~y + 1;
	/*checks if x - y has a different sign from x*/
	difSign = (newNum >> 31) ^ (x>>31);
	/*if the signs of x and y are the same then returns 1
	* if sign of x-y is the same as x then returns 1 as well
	*/
	return ((mightFail ^ 1) & 1) | (~difSign & 1);
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
	/*checks if x is 0*/
 	int isTrue = !x << 31 >> 31;
	/*if x is 0 then returns z and if x is not 0 then returns y*/
	return (~isTrue & y) | (isTrue & z);
}
/* 
 * replaceByte(x,n,c) - Replace byte n in x with c
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: replaceByte(0x12345678,1,0xab) = 0x1234ab78
 *   You can assume 0 <= n <= 3 and 0 <= c <= 255
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 3
 */
int replaceByte(int x, int n, int c) {
  	/*sifts n by 3 so it represents size of byte*/	
	n = n << 3;
	/*shifts the new byte to corrent location*/
	c = c << n;
	/*deletes old byte*/
	x = x & (~(255 << n));
	/*adds new byte to partially cleared x*/
	x = x + c;
	
	return x;
}
/*
 * satMul3 - multiplies by 3, saturating to Tmin or Tmax if overflow
 *  Examples: satMul3(0x10000000) = 0x30000000
 *            satMul3(0x30000000) = 0x7FFFFFFF (Saturate to TMax)
 *            satMul3(0x70000000) = 0x7FFFFFFF (Saturate to TMax)
 *            satMul3(0xD0000000) = 0x80000000 (Saturate to TMin)
 *            satMul3(0xA0000000) = 0x80000000 (Saturate to TMin)
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 3
 */
int satMul3(int x) {
    	int max = 1;
	int min = 0;
	int sign = 0;
	int aSign = 0;
	int midSign = 0;
	int sat = 0;
	int a = (x << 1); 
	/*multiplies by 3, broken up into steps.  Saves inbetween step.*/
	midSign = a >> 31;
	a = a + x;
	/*gets sign of x and inbetween number*/
	sign = x >> 31;
	aSign = a >> 31;
	
	/*sets tmin and tmax*/	
	min = max << 31;
	max = ~min;

	
	/*checks to see if any of the steps of multiplying by 3 cause an overflow*/
	sat = (sign ^ aSign)  | (aSign ^ midSign);
	/*saturates to tmin or tmax if any of the steps cause an overflow*/
	x = (a & ~sat) | (sat & ((sign & min) | (~sign & max)));
	return x;
}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
  
	int isSame = 0;
	
	int start = 0;
	int oneBit = 0;
	
	/*looks at left half of bit string*/
	int a = x >> 16;
	/*deletes one half of the string*/
	oneBit = !x | !~x;
	oneBit = oneBit << 31 >> 31;
	/*checks if all the numbers on one half are 1s or all 0s*/
	isSame = !a | !~a;
	isSame = isSame << 31 >> 31;
	/*changes where the bytes to look at depending on if they are the same or not
	* increments counter to store where first change is
	*/
	start =  (16 & ~isSame);
	
	/*repetes*/
	a = x >> (8 + start);
	isSame = !a | !~a;
	isSame = isSame << 31 >> 31;
	start = start + (8 & ~isSame);
	
	a = x >> (4 + start);
	isSame = !a | !~a;
	isSame = isSame << 31 >> 31;
	start = start + (4 & ~isSame);

	a = x >> (2 + start);
	isSame = !a | !~a;
	isSame = isSame << 31 >> 31;
	start = start + (2 & ~isSame);
	
	a = x >> (1 + start);
	isSame = !a | !~a;
	isSame = isSame << 31 >> 31;
	start = start + (1 & ~isSame);
	
	/*detects if is all 0, and returns the counter*/
	return ((start + 2) & ~oneBit) | (oneBit & 1);
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
	//printf("exp %d\n", exp);
	
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
