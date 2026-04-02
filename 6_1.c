#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define X 3



int main() {
	int sum = 0;
	int maxim = -2147483647;
	int minim = 2147483647;
	srand((unsigned int)time(NULL));
	int a = 0;
	scanf("%d", &a);
	int* aptr;
	aptr = (int*)malloc((a+1) * sizeof(int));
	int* ptr = aptr;

	for (int i = 0; i < a; i++) {
		*aptr = rand() % (a+1);
		if (*aptr > maxim) {
			maxim = *aptr;
		}
		if (*aptr < minim) {
			minim = *aptr;
		}
		sum += *aptr;
		aptr++;
	}
	aptr = ptr;
	for (int i = 0; i < a; i++) {
		printf("%d ", *aptr);
		aptr++;
	}
	aptr = ptr;
	printf("\n");
	for (int i = a; i > 0; i--) {
		printf("%d ", *aptr);
		aptr++;
	}

	printf("total: %d\n", sum);
	printf("maximum: %d\n", maxim);
	printf("minimum: %d\n", minim);


}
