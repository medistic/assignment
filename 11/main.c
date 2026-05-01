#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "polynomial.h"

int main() {
    char input1[256];
    char input2[256];

    printf("first polynomial: ");
    scanf("%255s", input1);

    printf("second polynomial ");
    scanf("%255s", input2);

    polynomial* p1 = createPolynomial(100);
    polynomial* p2 = createPolynomial(100);

    parsePolynomial(p1, input1);
    parsePolynomial(p2, input2);

	polynomial* p3 = addPolynomial(p1, p2);

	printf("P1 + P2 =");
	printPolynomial(p3);
	
	polynomial* p4 = multiplyPolynomial(p1, p2);
	
	printf("P1 * P2 =");
	printPolynomial(p4);
}
