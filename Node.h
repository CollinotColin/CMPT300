#pragma once

typedef struct __Node__
{
	void *item;
	struct __Node__ *next, *prev;
} Node;

Node* createNode(void * item);
void distoryNode(Node*, void(void*));