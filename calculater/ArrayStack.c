#include "ArrayStack.h"

ArrayStack* createArrayStack(int size, stackType type) {
	ArrayStack* re = (ArrayStack*)malloc(sizeof(ArrayStack));
	re->data = (stackElement*)malloc(sizeof(stackElement) * size);
	re->top = -1;
	re->size = size;
	re->type = type;

	return re;
}

int emptyArrayStack(ArrayStack* s) {
	return s->top == -1;
}

int fullArrayStack(ArrayStack* s) {
	return s->top == s->size - 1;
}

int pushArrayStack(ArrayStack* s, stackElement item) {
	if (fullArrayStack(s)) {
		return 0;
	}

	s->top++;

	if (s->type == CHARACTER) {
		s->data[s->top].operator = item.operator;
	}
	else if (s->type == INTEGER) {
		s->data[s->top].value = item.value;
	}

	return 1;
}

stackElement popArrayStack(ArrayStack* s) {
	if (emptyArrayStack(s)) {
		stackElement r;
		r.operator = 0;
		return r;
	}

	stackElement temp = s->data[s->top];
	s->top--;

	return temp;
}

int printArrayStack(ArrayStack* s) {
	printf("Stack:\n");
	printf("Size: %d, Top: %d\n", s->size, s->top);
	for (int i = 0; i <= s->top; i++) {
		if (s->type == CHARACTER)
			printf("%c ", s->data[i].operator);
		else if (s->type == INTEGER)
			printf("%d ", s->data[i].value);
	}
	printf("\n");
	return 0;
}

stackElement peekArrayStack(ArrayStack* s) {
	return s->data[s->top];
}