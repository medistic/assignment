#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#include "my_math.h"

int main() {
	char str1[1000];
	char str2[1000];
	int num1 = 0;
	int num2 = 0;
	int j = 0;
	char* endptr;
	char type[1] = { 'a' };
	int appro = 0;

	scanf("%s", str1);
	scanf("%s", str2);
	j = strtol(str1, &endptr, 10);
	if (*endptr != 0) {
		printf("Put an integer in the 1st fraction.");
		return 0;
	}

	for (int i = 0; i < sizeof(str1);i++) {
		if (str1[i] == '/') {
			printf("Not appropriate number");
			return 0;
		}
		if (str1[i] == '.') {
			printf("Not appropriate number");
			return 0;
		}
		
		else
		{
			num1 = atoi(str1);
		}
	}
	
	j = strtol(str2, &endptr, 10);
	if (*endptr != 0) {
		printf("Put an integer in the 2st fraction.");
		return 0;
	}
	for (int i = 0; i < sizeof(str2);i++) {
		if (str2[i] == '/') {
			printf("Not appropriate number");
			return 0;
		}
		if (str2[i] == '.') {
			printf("Not appropriate number");
			return 0;
		}
		
		else
		{
			num2 = atoi(str2);
		}
	}
	appro = GNNGN(num1, num2);
	if(appro == 1) {
		printf("Not appropriate number");
		return 0;
	}
	else
	{
		printf("GCD: %d, ", gcd(num1, num2));
		printf("LCM: %d\n", lcm(num1, num2));
	}
	
}