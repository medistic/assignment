#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#include "my_fraction.h"

int main() {
	fractions answer;
	answer.numer =  0;
	answer.den = 0;
	int testa = 0;
	fractions num1;
	num1.den = 0;
	num1.numer = 0;
	fractions num2;
	num2.den = 0;
	num2.numer = 0;
	char clist[1] = { 'a', };
	char opra = 'a';
	scanf(" %d%c%d",&num1.numer,&clist[0],&num1.den);
	scanf(" %c", &opra);
	scanf(" %d%c%d", &num2.numer, &clist[0], &num2.den);
	if (num1.den == 0)
	{
		printf("Put a non-zero value in the denominator of the 1st fraction.");
		return 0;

	}
	if (num2.den == 0)
	{
		printf("Put a non-zero value in the denominator of the 2nd fraction.");
		return 0;

	}
	testa = testb( num1,num2,clist[0]);
	if (testa == 1) {
		printf("out of range.");
		return 0;
	}
	
	if (opra = '+') {
		answer = add(num1, num2);
	}
	
	else if (opra = '-') {
		answer = minus(num1, num2);

	}
	else if (opra = '*') {
		answer = multi(num1, num2);
	}
	
	else if (opra = '/') {
		answer = divis(num1, num2);
	}
	answer = abbre(answer);
	if (answer.den == 1	){
		printf("%d", answer.numer);
		return 0;
	}
	printf(" %d/%d\n", answer.numer, answer.den);
}
