#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define N 30


int fib();
int main() {
	int n = N;
	int res = 0;
	int x = 1;
	scanf("%d", &n);
	res = fib(n);
	printf("recursional way %d th fib_num:%d\n", n, res);
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
