
#include <stdio.h>
#include <stdlib.h>

#include "my_fraction.h"

//통분하기

int denom(fractions i, fractions j) {
	int den = 0;
	den = i.den * j.den;
	return den;

}
//사칙연산
fractions add(fractions i, fractions j) {
	
	fractions answer;
	answer.den = 0;
	answer.numer = 0;
	answer.numer = i.numer * j.den + i.den * j.numer;
	answer.den = denom(i, j);
	
	

	return answer;
}
fractions minus(fractions i, fractions j) {

	fractions answer;
	answer.den = 0;
	answer.numer = 0;
	answer.numer = i.numer * j.den - i.den * j.numer;
	answer.den = denom(i, j);
	


	return answer;
}
fractions multi(fractions i, fractions j) {
	fractions answer;
	answer.den = 0;
	answer.numer = 0;
	answer.den = i.den * j.den;
	answer.numer = i.numer * j.numer;
	
	return answer;
}
fractions divis(fractions i, fractions j) {
	fractions answer;
	answer.den = 0;
	answer.numer = 0;
	answer.den = i.den * j.numer;
	answer.numer = i.numer * j.den;
	
	return answer;
}
extern fractions abbre(fractions i) {

	int abb = -1;
	int least = 2147483647;
	if (0 > i.den) {
		i.den = -i.den;
	}
	if (0 > i.numer) {
		i.numer = -i.numer;
	}
	if (least > i.den) {
		least = i.den;
	}
	if (least > i.numer) {
		least = i.numer;
	}
	if (least == 1) {
		return i;
	}
	while (abb != 1) {
		abb = 1;
		for (int j = 2; j < least;j++) {
			if ((i.den % j) == 0) {
				if ((i.numer % j) == 0) {
					abb = j;
				}
			}
		}
		i.numer = i.numer / abb;
		i.den = i.den / abb;
	}
	return i;
}
extern int testb(fractions i, fractions j , char a) {
	if (i.den < 0) {
		i.den = -i.den;
	}
	if (i.numer < 0) {
		i.numer = -i.numer;
	}
	if (j.den < 0) {
		j.den = -j.den;
	}
	if (j.numer < 0) {
		j.numer = -j.numer;
	}
	
	
		if ((2147483647 - i.den * j.numer) < j.den * i.numer) {
			return 1;
		}
		if(a =='*'){
			if ((2147483647 / i.den) < j.den) {
				return 1;
			}
			if ((2147483647 / i.numer) < j.numer) {
				return 1;
			}
		}
		if (a == '/') {
			if ((2147483647 / i.den) < j.numer) {
				return 1;
			}
			if ((2147483647 / i.numer) < j.den) {
				return 1;
			}
		}
		
		return 0;
	

}
