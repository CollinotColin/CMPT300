#include "Node.h"
#include "Memory.h"
Node * createNode(void * item)
{
	Node * node = allocNode();
	if (node == 0) 
		return 0;
	node->item = item;
	node->prev = node->next = 0;
	return node;
}

void distoryNode(Node * p , void func(void*))
{
	if (func!=0) 
		func(p->item);
	freeNode(p);
}
