#include <stdio.h>
#include <stdlib.h>


typedef int StackElement;
typedef struct node {
	StackElement data;
	struct node* next;

}node;
typedef struct LinkedStack {
	node* head;
	int size;
}LinkedStack;

extern LinkedStack* createLinkedStack();
extern int destroyLinkedStack(LinkedStack s);
extern int emptyLinkedStack(LinkedStack s);
extern int fullLinkedStack(LinkedStack s);
extern int pushLinkedStack(LinkedStack s,StackElement item);
extern int popLinkedStack(LinkedStack s);
extern int printLinkedStack(LinkedStack s);
extern StackElement peakLinkedStack(LinkedStack s);
