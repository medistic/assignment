#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define X 3



int main() {
	int sum = 0;
	int maxim= -2147483647;
	int minim = 2147483647;
	srand((unsigned int)time(NULL));
	int a = 0;
	scanf("%d", &a);
	int *aptr;
	aptr = (int *)malloc(a * sizeof(int));
	int* ptr = aptr;
	
	for (int i = 0; i < a; i++) {
		*aptr = rand() % a;
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
	for (int i = 0; i < X-1; i++) {
		printf("%d ", *aptr);
		aptr++;
	}
	aptr = ptr;
	printf("\n");
	for (int i = X-1; i > 0; i--) {
		printf("%d ", *aptr);
		aptr++;
	}

	printf("total: %d\n", sum);
	printf("maximum: %d\n", maxim );
	printf("minimum: %d\n", minim );
	
	
}
