#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>



int fib();
int main() {
	int n = 0;
	int res = 0;
	int x = 1;
	scanf("%d", &n);
	res = fib(n);
	printf("recursional way %d th fib_num:%d\n", n, res);
	res = 0;
	
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
int fib(int n) {
	int num=0;
	if (n == 1) {
		return 1;
	}
	else if (n == 2) {
		return 1;
	}
	else {
		num = fib(n - 1) + fib(n - 2);
		printf("%d th fib_num: %d\n",n, num);
	}
	return num;
}
