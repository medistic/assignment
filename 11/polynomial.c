#include "polynomial.h"

polynomial* createPolynomial(int capacity) {
	polynomial* po = (polynomial*)createArrayList(capacity);
	return po;
}

polynomial* addItemPolynomial(polynomial* po, elementType item) {
	int i;
	for (i = 0; i < sizeArrayList(po); i++) {
		if (po->data[i].expo <= item.expo) {
			break;
		}
	}

	if (i == sizeArrayList(po)) {
		insertArrayList(po, i, item);
	}
	else if (po->data[i].expo == item.expo) {
		po->data[i].coef += item.coef;
	}
	else {
		insertArrayList(po, i, item);
	}

	return po;
}

polynomial* addPolynomial(polynomial* p1, polynomial* p2) {
	polynomial* r = createPolynomial(100);

	int i = 0, j = 0;

	while (i < sizeArrayList(p1) && j < sizeArrayList(p2)) {
		if (p1->data[i].expo < p2->data[j].expo) {
			addItemPolynomial(r, p2->data[j]);
			j++;
		}
		else if (p1->data[i].expo > p2->data[j].expo) {
			addItemPolynomial(r, p1->data[i]);
			i++;
		}
		else {
			addItemPolynomial(r, (elementType) {
				p1->data[i].coef + p2->data[j].coef,
				p1->data[i].expo
			});

			i++;
			j++;
		}
	}
	while (i < sizeArrayList(p1)) {
		addItemPolynomial(r, p1->data[i]);
		i++;
	}

	
	while (j < sizeArrayList(p2)) {
		addItemPolynomial(r, p2->data[j]);
		j++;
	}
	return r;
}

void printPolynomial(polynomial* po) {
	printf("Polynomial: \n");

	printArrayList(po);

	printf("\n");
}

void parsePolynomial(polynomial* p, char* str) {
	char* token = strtok(str, "+");
	while (token != NULL) {
		int coef, expo;
		// "%dX^%d" 형식으로 계수와 지수 추출
		if (sscanf(token, "%dX^%d", &coef, &expo) == 2) {
			addItemPolynomial(p, (elementType) { coef, expo });
		}
		token = strtok(NULL, "+");
	}
}

polynomial* multiplyPolynomial(polynomial* p1, polynomial* p2) {
	polynomial* r = createPolynomial(200);

	for (int i = 0; i < sizeArrayList(p1); i++) {
		for (int j = 0; j < sizeArrayList(p2); j++) {
			elementType newTerm;
			newTerm.coef = p1->data[i].coef * p2->data[j].coef;
			newTerm.expo = p1->data[i].expo + p2->data[j].expo;

			addItemPolynomial(r, newTerm);
		}
	}

	return r;
}