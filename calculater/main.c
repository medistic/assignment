#include "calculator.h"

int main() {
	char sample[] = "3+4*(5+6)*7";
	char postfix[100];

	infixToPostfix(sample, postfix);
	printf("%s\n", postfix);
	int result = evalPostfix(postfix);
	printf("Result: %d\n", result);
}
