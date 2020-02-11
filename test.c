#include "LIST.h"
#include "Memory.h"
#include <stdio.h>

int comp(void* a, void * b)
{
	return (*(int*)a) == (*(int*)b);
}

void showasint(void* x)
{
	printf("%d ", *(int*)x);
}

int main()
{
	int a[100],i;
	memoryInit();
	LIST * list = ListCreate();

	/*
	for (i = 0; i < 100; i++) a[i] = i;
	LIST * list = ListCreate();
	for (int i = 0; i < 20; i++)
		if (ListAppend(list, (void*)&a[i]) < 0)
		{
			printf("Error When Appending\n");
		}
		else
		{
			printf("a[%d] is appended.\n", i);
		}
	for (ListFirst(list); ListCurr(list) != 0; ListNext(list))
	{
		printf("%d ", *(int*)ListCurr(list));
	}
	printf("\n");
	int b = 5;
	ListFirst(list);
	if (ListSearch(list, comp, &b))
	{
		printf("%d\n", *(int*)ListCurr(list));
	}
	else
	{
		printf("Not found\n");
	}
	ListFree(list, showasint);
	printf("\n");
	LIST * llist[5] = {0};
	for (i = 0; i < 5; i++)
	{
		llist[i] = ListCreate();
		if (llist[i] == 0)
		{
			printf("list alloc failed\n");
		}
		else
		{
			printf("list alloc success\n");
		}
	}
	*/

	return 0;
}