#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define N 30



int main() {
	int n = N;
	int res = 0;
	int x = 1;
	int fibn_2 = 1;
	int fibn_1 = 1;
	while (x < n) {
		x++;
		if (x == 1) {
			res= 1;
			continue;
		}
		else if (x == 2) {
			res = 1;
			continue;
		}
		
		res = fibn_1 + fibn_2;
		fibn_2 = fibn_1;
		fibn_1 = res;
	}
	printf("Iterational way %d th fib_num:%d\n", n, res);
}
