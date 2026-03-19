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
	int i = 0;
	int j = 0;
	int xnumer = 0;
	int xden = 1;
	float x = 0.0;
	float y = 0.0;
	int file = -1;
	while (file >= 3 || file < 0) {
		printf("Enter the number of the file you want to read from (fraction:0, integer:1, float:2\n type: ");
		scanf(" %d", &file);
		if (file >= 3 || file < 0) {
			printf("put number between 0,3");
			continue;
		}
	}
	if (file == 0) {
		scanf(" %d%c%d", &num1.numer, &clist[0], &num1.den);
		scanf(" %c", &opra);
		scanf(" %d%c%d", &num2.numer, &clist[0], &num2.den);
		printf(" %d/%d\n", num1.numer, num1.den);
		printf(" %d/%d\n", num2.numer, num2.den);
	}
	else if (file == 1) {
		scanf(" %d", &i);
		scanf(" %c", &opra);
		scanf(" %d", &j);
		num1.den = 1;
		num1.numer = i;
		num2.den = 1;
		num2.numer = j;
		printf(" %d/%d\n", num1.numer, num1.den);
		printf(" %d/%d\n", num2.numer, num2.den);
	}
	else if (file == 2) {
		scanf(" %f", &x);
		scanf(" %c", &opra);
		scanf(" %f", &y);
		for (int i = 0;i < 23;i++){
			xnumer = x;
			if (x - xnumer != 0.0) {
				x = x * 10;
				xden = xden * 10;
			}
			else {
				num1.den = xden;
				num1.numer = xnumer;
			}
		}
		for (int i = 0;i < 23;i++) {
			xnumer = y;
			if (y - xnumer != 0.0) {
				y = y * 10;
				xden = xden * 10;
			}
			else {
				num2.den = xden;
				num2.numer = xnumer;
			}
		}
	}
	
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
	
	if (opra == '+') {
		answer = add(num1, num2);
	}
	
	if (opra == '-') {
		answer = minus(num1, num2);

	}
	if (opra == '*') {
		answer = multi(num1, num2);
	}
	
	if (opra == '/') {
		answer = divis(num1, num2);
	}
	
	answer = abbre(answer);
	if (answer.den == 1	){
		printf("%d", answer.numer);
		return 0;
	}
	printf(" %d/%d\n", answer.numer, answer.den);
}
