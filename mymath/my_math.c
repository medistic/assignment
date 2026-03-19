#include <stdio.h>
#include <stdlib.h>

#include "my_math.h"

extern int GNNGN(int i, int j) {
	if (i < 0){
		return 1;
	}
	if (i < 0) {
		return 1;
	}

	return 0;
}
extern int lcm(int i, int j) 
	{
		int r;
		while (j != 0) {
			r = i % j;
			i = j;
			j = r;
		}
		return i;
	}


extern int gcd(int i, int j) {
	int gcd = 0;
	gcd = (i * j) / lcm(i, j);
	return gcd;
}
