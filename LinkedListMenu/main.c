#define _CRT_SECURE_NO_WARNINGS
#include "LinkedList.h"

int main() {
	struct linkedList* myList1;
	myList1 = createLinkedList();

	int menu = -1;
	
	do{
		scanf(" %d", &menu);
		if (menu == 1) {
			PointType item;
			scanf(" %d", &item.x);
			insertFirstLinkedList(myList1, item);
		}
		else if (menu == 2) {

			PointType item;
			scanf(" %d", &item.x);
			if (sizeLinkedList(myList1) == 0) {
				insertFirstLinkedList(myList1, item);
				continue;
			}
			insertLastLinkedList(myList1, item);
		}
		else if (menu == 3) {
			PointType item;
			int at;
			scanf(" %d", &item.x);
			scanf(" %d", &at);
			if (at == 0) {
				insertFirstLinkedList(myList1, item);
				continue;
			}
			PointType* nPtr = myList1->head;

			for (int i = 0; i < at - 1; i++) {
				nPtr = nPtr->next;
			}
			insertItemLinkedList(myList1, nPtr, item);
		}
		else if (menu == 4) {
			int at;
			scanf(" %d", &at);
			deleteAtLinkedList(myList1, at);

		}
		else if (menu == 5) {
			printLinkedList(myList1);
		}
		else if (menu == 6) {
			printf("LinkedList size: %d\n", sizeLinkedList(myList1));
		}
		else if (menu == 0) {
			destroyLinkedList(myList1);
		}
	} while (menu != 0);
}
