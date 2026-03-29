#include <stdio.h>
#include <stdlib.h>


int main() {
	
	int a = 12;
	int b = 18;
	int r = 0;

	while (b != 0) {
		r = a%b;
		a = b;
		b = r;
	}
	
	printf("GCD = %d\n", a);
}
