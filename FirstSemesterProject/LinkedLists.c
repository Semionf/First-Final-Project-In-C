#include "LinkedLists.h"

t_snapShot* head = NULL;
t_snapShot* tail = NULL;

t_snapShot* addToList(t_snapShot* snapShot) // making snapshot list
{
	if (!snapShot)
	{
		head = tail = NULL;
		return NULL;
	}
	else if (!head)
	{
		snapShot->next = snapShot->prev = NULL;
		head = tail = snapShot;
	}
	else {
		tail->next = snapShot;
		snapShot->prev = tail;
		snapShot->next = NULL;
		tail = snapShot;
	}
	return head;
}