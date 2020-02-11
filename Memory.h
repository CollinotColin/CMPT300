#pragma once
#include "LIST.h"
#include "Node.h"
#define MAXHEADS 3
#define MAXNODES 15

void memoryInit();
LIST* allocHead();
Node* allocNode();
void freeHead(LIST*);
void freeNode(Node*);

