#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	char type[1] = { 'a' };
	int end = 0;
	char str1[1000];
	char str2[1000];
	char clist[1] = { 'a', };
	char opra = 'a';
	char* dvd;
	float x = 0.0;
	int xnumer = 0;
	int xden = 1;
	long j = 0;
	char* endptr;

	scanf("%s", str1);
	scanf(" %c", &opra);
	scanf("%s", str2);
	
	
	
	for (int i = 0; i < sizeof(str1);i++) {
		if (str1[i] == '/') {
			type[0] = 'f';
			break;
		}
		if (str1[i] == '.') {
			type[0] = 'F';
			break;
		}
		
		else
		{
			
			type[0] = 'i';
		}

	}
	if (type[0] != 'f' && type[0] != 'F') {
		j = strtol(str1, &endptr, 10);
		if (*endptr != 0) {
			printf("Put an integer in the 1st fraction.");
			return 0;
		}
	}
	if(type[0]=='f'){
		dvd = strtok(str1, "/");
		if (dvd != NULL) {
			num1.numer = atoi(dvd);
			dvd = strtok(NULL, "/");
			num1.den = atoi(dvd);
		}
	}
	
	if (type[0] == 'i') {
		num1.den = 1;
		num1.numer = atoi(str1);
	}

	if (type[0] == 'F') {
		
		x = atof(str1);
		
		for (int i = 0;i < 23;i++){
			xnumer = x;
			if (x - xnumer != 0.0) {
				x = x * 10;
				xden = xden * 10;
				
				
				continue;
			}
			else {
				num1.den = xden;
				num1.numer = xnumer;
				
			}
		}

	}
	
	for (int i = 0; i < sizeof(str2);i++) {
		if (str2[i] == '/') {
			type[0] = 'f';
			break;
		}
		if (str2[i] == '.') {
			type[0] = 'F';
			break;
		}
		
		else
		{
			
			type[0] = 'i';
		}
	}
	if (type[0] != 'f' && type[0] != 'F') {
		j = strtol(str2, &endptr, 10);
		if (*endptr != 0) {
			printf("Put an integer in the 2st fraction.");
			return 0;
		}
	}
	if (type[0] == 'f') {
		dvd = strtok(str2, "/");
		if (dvd != NULL) {
			num2.numer = atoi(dvd);
			dvd = strtok(NULL, "/");
			num2.den = atoi(dvd);
		}
	}

	if (type[0] == 'i') {
		num2.den = 1;
		num2.numer = atoi(str2);
	}

	if (type[0] == 'F') {

		x = atof(str2);

		for (int i = 0;i < 23;i++) {
			xnumer = x;
			if (x - xnumer != 0.0) {
				x = x * 10;
				xden = xden * 10;

				
				continue;
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
