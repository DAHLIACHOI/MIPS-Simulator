#include <stdio.h>
#include <stdlib.h>


// Arithmetic unit
int addSubtract(int X, int Y, int C) {
	if (C < 0 || C > 1) {
		printf("error in addSubtract()...\n");
		return 0;
	}
	if (C == 0)
		return X + Y;
	else
		return X - Y;
}

// Logic unit
int logicOperation(int X, int Y, int C)
{
	if (C < 0 || C > 3) {
		printf("error in logicOperation()...\n");
		return 0;
	}

	if (C == 0)
		return X & Y;	// bitwise-AND
	else if (C == 1)
		return X | Y;   // bitwise-OR
	else if (C == 2)
		return X ^ Y;   // bitwise-XOR
	else
		return ~(X | Y); // bitwise-NOR
}

// Shift unit
// V: 5 bit shift amount
int shiftOperation(int V, int Y, int C)
{
	int ret;

	if (C < 0 || C > 3 || V > 31) {
		printf("error in shiftOperation()...\n");
		return 0;
	}

	if (C == 0) {	// No shift
		ret = Y;
	}
	else if (C == 1) {	// Logical left
		ret = Y << V;
	}
	else if (C == 2) {	// Logical right
		ret = (unsigned int)Y >> V;
	}
	else {	// Arithmetic right
		ret = Y >> V;
	}

	return ret;
}

int checkSetLess(int X, int Y)
{
	if (X < Y) return 1;
	else return 0;
}

int isZero(int S) {
    return S == 0;
}

int checkZero(int S, int* Z) {
    if (Z != NULL) *Z = isZero(S);
    return S;
}

// ALU

int ALU(int X, int Y, int C, int* Z) {
	int c32, c10,c0;
	int ret;

	c32 = (C >> 2) & 3;
	c10 = C & 3;		
	c0 = C & 1;

	if (c32 == 0) { // shift

		return shiftOperation(X, Y, c10);
	}
	else if (c32 == 1) {  // set less
		return checkSetLess(X, Y);
	}
	else if (c32 == 2) {  // addsubtract
		return checkZero(addSubtract(X, Y, c0), Z);
	}
	else {  // logic
		return logicOperation(X, Y, c10);
	}
}