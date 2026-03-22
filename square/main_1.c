#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "square.h"

int main(int c, char** v) {
	if (c != 3) {
		printf("Usage: %s <num1> <num2>\n", v[0]);
		return 1;
	}


	char type[1];
	char* endptr;
	fractions num1;
	fractions num2;
	num1.den = 0;
	num1.numer = 0;
	num2.den = 0;
	num2.numer = 0;
	fractions answer1;
	fractions answer2;
	answer1.den = 0;
	answer1.numer = 0;
	answer2.den = 0;
	answer2.numer = 0;
	int j = 0;
	char* dvd = 0;
	float x = 0;
	int xden = 1;
	int xnumer = 0;
	int testa = 0;



	for (int i = 0; i < strlen(v[1]);i++) {
		if (v[1][i] == '/') {
			type[0] = 'f';
			break;
		}
		if (v[1][i] == '.') {
			type[0] = 'F';
			break;
		}

		else{

			type[0] = 'i';
		}
	}
	if (type[0] != 'f' && type[0] != 'F') {
		j = strtol(v[1], &endptr, 10);
		if (*endptr != 0) {
			printf("Put an integer in the 2st fraction.");
			return 0;
		}
	}
	if (type[0] == 'f') {
		dvd = strtok(v[1], "/");
		if (dvd != NULL) {
			num1.numer = atoi(dvd);
			dvd = strtok(NULL, "/");
			num1.den = atoi(dvd);
		}
	}

	if (type[0] == 'i') {
		num1.den = 1;
		num1.numer = atoi(v[1]);
	}

	if (type[0] == 'F') {

		x = atof(v[1]);

		for (int i = 0;i < 23;i++) {
			xnumer = x;
			if (x - xnumer != 0.0) {
				x = x * 10;
				xden = xden * 10;


				continue;
			}
			else {
				num1.den = xden;
				num1.numer = xnumer;
				break;
			}

		}
		xden = 1;
	}
	for (int i = 0; i < strlen(v[2]);i++) {
		if (v[2][i] == '/') {
			type[0] = 'f';
			break;
		}
		if (v[2][i] == '.') {
			type[0] = 'F';
			break;
		}

		else
		{

			type[0] = 'i';
		}
	}
	if (type[0] != 'f' && type[0] != 'F') {
		j = strtol(v[2], &endptr, 10);
		if (*endptr != 0) {
			printf("Put an integer in the 2st fraction.");
			return 0;
		}
	}
	if (type[0] == 'f') {
		dvd = strtok(v[2], "/");
		if (dvd != NULL) {
			num2.numer = atoi(dvd);
			dvd = strtok(NULL, "/");
			num2.den = atoi(dvd);
		}
	}

	if (type[0] == 'i') {
		num2.den = 1;
		num2.numer = atoi(v[2]);
	}

	if (type[0] == 'F') {

		x = atof(v[2]);

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
				break;
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
			testa = testb(num1, num2);
			if (testa == 1) {
				printf("out of range.");
				return 0;
			}
			answer1 = multi(num1, num2);
			answer2 = 2*(add(num1, num2));
			answer1 = abbre(answer1);
			answer2 = abbre(answer2);
		
			if (answer1.den == 1) {
				printf("circumference: %d", answer1.numer);
			}
			else if (answer1.den != 1) {
				printf("circumference: %d/%d\n", answer1.numer, answer1.den);
			}
			if (answer2.den == 1) {
				printf("extend: %d", answer2.numer);
			}
			
			else if (answer2.den != 1) {
				printf("extend: %d/%d\n", answer2.numer, answer2.den);
			}



		
	
}
