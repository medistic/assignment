
#define _CRT_SECURE_NO_WARNINGS
#include "LinkedStack.h"

int main()
{
	int input = 0;
	char inputItem[2048];


	LinkedStack* URLs = createLinkedStack();
	do {
		scanf("%d", &input);
		if (input == 1) {
			printf("input URLs:");
			scanf("%s", inputItem);
			pushLinkedStack(URLs, inputItem);
			printf("%s\n", peakLinkedStack(URLs));
		}
		else if (input == 2) {
			if (!emptyLinkedStack(URLs)) {
				StackElement item = popLinkedStack(URLs);
				printf("%s\n", item);
				free(item);
			}
			else
			{
				printf("Stack is empty\n");
				continue;
			}
			if (emptyLinkedStack(URLs) == 1) {
				destroyLinkedStack(URLs);
				return 0;
			}
			printf("%s\n", peakLinkedStack(URLs));
		}
		else if (input == 3) {
			if (emptyLinkedStack(URLs)) {
				continue;
			}
			printf("%s\n", peakLinkedStack(URLs));
		}

	} while (1);
}
