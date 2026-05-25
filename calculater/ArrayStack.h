#include <stdio.h>
#include <stdlib.h>

typedef enum {
	CHARACTER, INTEGER
} stackType;

typedef union {
	char operator;
	int value;
} stackElement;

typedef struct arrayStack {
	stackElement* data;
	int top;
	int size;
	stackType type;
} ArrayStack;

extern ArrayStack* createArrayStack(int size, stackType type);
extern int pushArrayStack(ArrayStack* s, stackElement item);
extern stackElement popArrayStack(ArrayStack* s);
extern int printArrayStack(ArrayStack* s);
extern int emptyArrayStack(ArrayStack* s);
extern int fullArrayStack(ArrayStack* s);
extern stackElement peekArrayStack(ArrayStack* s);

// extern int destroyArrayStack(ArrayStack* s);