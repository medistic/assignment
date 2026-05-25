#include "calculator.h"
#include "ArrayStack.h"

int infixToPostfix(char* infix, char* postfix) {
	int j = 0;
	ArrayStack* opStack = createArrayStack(100, CHARACTER);

	for (int i = 0; infix[i] != '\0'; i++) {
		if (infix[i] >= '0' && infix[i] <= '9') {
			postfix[j] = infix[i];
			j++;
		}
		else if (infix[i] == '+' || infix[i] == '-' ||
			infix[i] == '*' || infix[i] == '/') {

			while (!emptyArrayStack(opStack)) {
				stackElement prevOp;
				prevOp.operator = peekArrayStack(opStack).operator;

				if (precedence(infix[i]) <= precedence(prevOp.operator)) {
					postfix[j] = popArrayStack(opStack).operator;
					j++;
					printf("postfix: %s\n", postfix);
				
					printArrayStack(opStack);
				}
				else {
					break;
				}
			}
			stackElement item;
			item.operator = infix[i];
			pushArrayStack(opStack, item);
			printf("postfix: %s\n", postfix);
			
			printArrayStack(opStack);
		}
		else if (infix[i] == '(') {
			stackElement item;
			item.operator = infix[i];
			pushArrayStack(opStack, item);
			printf("postfix: %s\n", postfix);
			
			printArrayStack(opStack);
		}
		else if (infix[i] == ')') {
			do
			{
				stackElement prevOp = popArrayStack(opStack);
				
				if (prevOp.operator != '(') {
					postfix[j] = prevOp.operator;
					j++;
					printf("postfix: %s\n", postfix);
					
					printArrayStack(opStack);
				}
				if (prevOp.operator == '(') {

					break;
				}
			} while (!emptyArrayStack(opStack));
			printf("postfix: %s\n", postfix);
			
			printArrayStack(opStack);
		}
	}
	while (!emptyArrayStack(opStack)) {
		postfix[j] = popArrayStack(opStack).operator;
		j++;
		printf("postfix: %s\n", postfix);
		
		printArrayStack(opStack);
	}
	postfix[j] = '\0';
	return 0;
}

int evalPostfix(char* postfix) {
	ArrayStack* valueStack = createArrayStack(100, INTEGER);
	int i = 0;
	printf("postfix: %s\n", postfix);
	while (postfix[i] != '\0') {
		if (postfix[i] >= '0' && postfix[i] <= '9') {
			stackElement valueElement;
			valueElement.value = (int)(postfix[i] - '0');
			pushArrayStack(valueStack, valueElement);
			printf("current: %c\n", postfix[i]);
			printArrayStack(valueStack);
		}
		else if (postfix[i] == '*') {
			stackElement i1 = popArrayStack(valueStack);
			stackElement i2 = popArrayStack(valueStack);
			stackElement item;
			item.value = i2.value* i1.value;
			pushArrayStack(valueStack, item);
			printf("current: %c\n", postfix[i]);
			
			printArrayStack(valueStack);
		}
		else if (postfix[i] == '/') {
			stackElement i1 = popArrayStack(valueStack);
			stackElement i2 = popArrayStack(valueStack);
			stackElement item;
			item.value = i2.value / i1.value;
			pushArrayStack(valueStack, item);
			printf("current: %c\n", postfix[i]);
			
			printArrayStack(valueStack);
		}
		else if (postfix[i] == '+') {
			stackElement i1 = popArrayStack(valueStack);
			stackElement i2 = popArrayStack(valueStack);
			stackElement item;
			item.value = i2.value + i1.value;
			pushArrayStack(valueStack, item);
			printf("current: %c\n", postfix[i]);
			
			printArrayStack(valueStack);
		}
		else if (postfix[i] == '-') {
			stackElement i1 = popArrayStack(valueStack);
			stackElement i2 = popArrayStack(valueStack);
			stackElement item;
			item.value = i2.value - i1.value;
			pushArrayStack(valueStack, item);
			printf("current: %c\n", postfix[i]);
			
			printArrayStack(valueStack);
		}

		i++;
	}

	return popArrayStack(valueStack).value;
}

int precedence(char op) {
	switch (op)
	{
	case ')':
		return 3;
	case '*':
	case '/':
		return 2;
	case '+':
	case '-':
		return 1;
	case '(':
		return 0;
	default:
		return 0;
	}
}
