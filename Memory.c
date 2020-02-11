#include "Memory.h"
#include<assert.h>
LIST headMemory[MAXHEADS];
Node nodeMemory[MAXNODES];

LIST* emptyHeads[MAXHEADS];
Node* emptyNodes[MAXNODES];
int usedHeads, usedNodes;

void memoryInit()
{
	int i = 0;
	for (i = 0; i < MAXHEADS; i++)
		emptyHeads[i] = headMemory+i;

	for (i = 0; i < MAXNODES; i++)
		emptyNodes[i] = nodeMemory+i;
	usedHeads = usedNodes = 0;
}

LIST * allocHead()
{
	if (usedHeads == MAXHEADS)// no more head 
		return 0;
	return emptyHeads[usedHeads++];
}

Node * allocNode()
{
	if (usedNodes == MAXNODES)// no more node 
		return 0;
	return emptyNodes[usedNodes++];
}

void freeHead(LIST* p)
{
	assert(usedHeads > 0);
	emptyHeads[--usedHeads] = p;
}

void freeNode(Node* p)
{
	assert(usedNodes > 0);
	emptyNodes[--usedNodes] = p;
}



