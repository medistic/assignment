#include <stdio.h>
#include <stdlib.h>

int gcd(int a, int b) {
	int r = 0;
	while (b != 0) {
		r = a % b;
		a = b;
		b = r;
	}
	return a;
	
}

int main() {
	long long a = 5;
	long long b = 3;
	long long r = 0;
	long long GCD = 0;
	long long x = 5;
	while (1) {
		x++;
		r = a + b;
		b = a;
		a = r;
		GCD = gcd(a, b);
		printf("%lld th Fnum:%lld\n ", x,a);
		printf("%lld th Fnum:%lld\n ", x-1,b);
		printf("GCD :%lld\n ", GCD);
		if (x == 100) {
			break;
		}
	}
	
}
