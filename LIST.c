#include "LIST.h"
#include "Node.h"
#include "Memory.h"
LIST * ListCreate()
{
	LIST* list= allocHead();
	if (list == 0) 
		return 0; //alloc failed
	list->first = list->curr = createNode(0);//super head
	list->last = createNode(0); //super tail
	if (list->first == 0 || list->last == 0) // alloc failed
	{
		freeHead(list);
		return 0;
	}
	list->first->next = list->last; //头尾相接
	list->last->prev = list->first; 
	list->count = 0;
	return list;
}

int ListCount(LIST * list)
{
	return list->count;
}

void * ListFirst(LIST *list)
{
	list->curr = list->first->next;
	return list->curr->item;
	//return (list->curr = list->first->next)->item;
}

void * ListLast(LIST *list)
{
	list->curr = list->last->prev;
	return list->curr->item;
	//return (list->curr = list->last->prev)->item;
}

void * ListNext(LIST *list)
{
	if (list->curr->next == 0)
		 return 0; // curr==super tail
	list->curr = list->curr->next;
	return list->curr->item;
	//return list->curr->next == 0 ? 0 : (list->curr = list->curr->next)->item;
}

void * ListPrev(LIST *list)
{
	if (list->curr->prev == 0) return 0;// curr==super head
	list->curr = list->curr->prev;
	return list->curr->item;
	//return list->curr->prev == 0 ? 0 : (list->curr = list->curr->prev)->item;
}

void * ListCurr(LIST *list)
{
	return list->curr->item;
}

int ListAdd(LIST * list, void * item)
{
	if (list->curr != list->last) // not beyond the end of list
	{
		Node* node = createNode(item);
		if (node == 0)
			return -1;
		list->count++;
		node->prev = list->curr;
		node->next = list->curr->next;
		node->prev->next = node;
		node->next->prev = node;

		list->curr=node;
	} //向后加
	else
		return ListInsert(list, item); //向前加
	return 0;
}

int ListInsert(LIST * list, void * item)
{
	if (list->curr != list->first) // not before the start of list
	{
		Node* node = createNode(item);
		if (node == 0)	return -1;
		list->count++;
		node->next = list->curr;
		node->prev = list->curr->prev;
		node->prev->next = node;
		node->next->prev = node;

		list->curr=node;
	} //向前加
	else
		return ListAdd(list, item); //向后加
	return 0;
}

int ListAppend(LIST *list, void *p)
{
	ListLast(list);
	return ListAdd(list, p);
}

int ListPrepend(LIST * list, void *p)
{
	ListFirst(list);
	return ListInsert(list,p);
}

void * ListRemove(LIST *list)
{
	Node * temp = list->curr;
	void* item = ListCurr(list);
	temp->next->prev = temp->prev;
	temp->prev->next = temp->next;
	list->curr = temp->next;
	freeNode(temp);
	list->count--;
	return item;
}

void ListConcat(LIST * list1, LIST *list2)
{
// remember to free the head of list2
	list1->last->prev->next = list2->first->next;
	list2->first->next->prev = list1->last->prev;
	distoryNode(list1->last, 0); //超级尾巴
	distoryNode(list2->first,0); //超级头头
	list1->count += list2->count;
	freeHead(list2);                  //可能有bug!!!
}

void ListFree(LIST * list, void func(void *))
{
	Node *temp, *nextNode;
	for (temp = list->first; temp != 0; temp = nextNode)
	{
		nextNode = temp->next;
		if (temp == list->first || temp == list->last)
			distoryNode(temp, 0); //超级尾巴或超级头头
		else
			distoryNode(temp, func); //普通node
	}
	freeHead(list);
}

void * ListTrim(LIST * list)
{
	ListLast(list);
	void *ret = ListRemove(list);
	ListLast(list);
	return ret;
}

void * ListSearch(LIST * list, int comp(void *,void *), void * compArg)
{
	if (list->curr == list->first)
   		ListFirst(list);
	for ( ; list->curr != list->last && !comp((list->curr->item),compArg);)
		ListNext(list);
	if(list->curr == list->last)
		return 0;
	else
		return list->curr->item;
}

