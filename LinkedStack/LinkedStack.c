#include "Linke.Stack.h"

extern LinkedStack* createLinkedStack() {
	LinkedStack* s;
	s->head = NULL;
	s->size = 0;
	return s;
}
extern int destroyLinkedStack(LinkedStack* s) {
	while (s->size != 0) {
		popLinkedStack(s);
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
	temp->data = item;
	temp->next = s->head;
	s->head = temp;
	s->size++;
	return 0;
}
extern int popLinkedStack(LinkedStack* s) {
	node* temp = s->head;
	s->head = s->head->next;
	free(temp);
	s->size--;
	return 0;
}
extern int printLinkedStack(LinkedStack* s) {
	node* temp=s->head;
	while(temp!=NULL){
	printf("%d", temp->data);
	temp = temp->next;
}
	return 0;
}
StackElement peakLinkedStack(LinkedStack* s) {
	return s->head->data;
}