#include "LinkedStack.h"

extern LinkedStack* createLinkedStack() {
	LinkedStack* s = (LinkedStack*)malloc(sizeof(LinkedStack));
	s->head = NULL;
	s->size = 0;
	return s;
}
extern int destroyLinkedStack(LinkedStack* s) {
	while (s->size != 0) {
		free(popLinkedStack(s));
	}
	free(s);
	return 0;
}
extern int emptyLinkedStack(LinkedStack* s) {
	if (s->size == 0) {
		return 1;
	}
	return 0;
}
extern int fullLinkedStack(LinkedStack* s) {
	return 0;
}
extern int pushLinkedStack(LinkedStack* s, StackElement item) {
	node* temp = (node*)malloc(sizeof(node));
	temp->data = (StackElement)malloc(strlen(item) + 1);
	strcpy(temp->data, item);
	temp->next = s->head;
	s->head = temp;
	s->size++;
	return 0;
}
StackElement popLinkedStack(LinkedStack* s) {
	node* temp = s->head;
	s->head = s->head->next;
	StackElement item = (StackElement)malloc(strlen(temp->data) + 1);
	strcpy(item, temp->data);
	free(temp->data);
	free(temp);
	s->size--;
	return item;
}
extern int printLinkedStack(LinkedStack* s) {
	node* temp = s->head;
	while (temp != NULL) {
		printf("%s", temp->data);
		temp = temp->next;
	}
	return 0;
}
StackElement peakLinkedStack(LinkedStack* s) {
	return s->head->data;
}
