#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define X 30



int main() {
	int sum = 0;
	int maxim = -2147483647;
	int minim = 2147483647;
	srand((unsigned int)time(NULL));


	int N[X] = { 0 };
	for (int i = 0; i < X; i++) {
		N[i] = rand() % X;
		if (N[i] > maxim) {
			maxim = N[i];
		}
		if (N[i] < minim) {
			minim = N[i];
		}
		sum += N[i];

	}

	for (int i = 0; i < X ; i++) {
		printf("%d ", N[i]);
	}
	printf("\n");
	for (int i = X - 1; i >= 0; i--) {
		printf("%d ", N[i]);
	}

	printf("total: %d\n", sum);
	printf("maximum: %d\n", maxim);
	printf("minimum: %d\n", minim);


}
