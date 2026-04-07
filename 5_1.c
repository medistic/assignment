#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define X 30
#define X 7



@@ -15,7 +15,7 @@ int main() {

	int N[X] = { 0 };
	for (int i = 0; i < X; i++) {
		N[i] = rand() % X;
		N[i] = rand() % (X+1);
		if (N[i] > maxim) {
			maxim = N[i];
		}
@@ -33,7 +33,7 @@ int main() {
	for (int i = X - 1; i >= 0; i--) {
		printf("%d ", N[i]);
	}

	printf("\n");
	printf("total: %d\n", sum);
	printf("maximum: %d\n", maxim);
	printf("minimum: %d\n", minim);
