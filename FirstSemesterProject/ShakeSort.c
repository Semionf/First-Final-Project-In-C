#include "ShakeSort.h"

t_snapShot* headOfProcesses;
void shakeSort(t_snapShot* head)
{
	t_Process* curr = head->process;
	headOfProcesses = head;
	if (!curr)
	{
		LogEvent("The list is empty.");
		return;
	}
	LogEvent("Sorting List");
	int i = 0;
	int j = 0;
	while (curr && i < head->processCounter)
	{
		while (curr && j < head->processCounter - 1 - i)
		{
			if (curr->numOfDLL > curr->next->numOfDLL)
			{
				replace(curr);
				curr = curr->prev;
			}
			curr = curr->next;
			j++;
		}
		curr = curr->prev;
		while (j > i + 1)
		{
			curr = curr->prev;
			if (curr->numOfDLL > curr->next->numOfDLL)// 1 3 2 4 6 5 7 9 8 10
			{
				replace(curr);
				curr = curr->prev;
			}
			j--;
		}
		i++;
		curr = curr->next;
	}
}

void replace(t_Process* process)
{
	if (!process) // no nodes.
	{
		LogEvent("Nothing to replace, list is empty");
		return;
	}
	if (!process->next) // one node or last node.
	{
		LogEvent("Nothing to replace (one node or last node)");
		return;
	}
	char str[100];
	sprintf(str, "Replacing %d with %d", process->numOfDLL, process->next->numOfDLL);
	LogEvent(str);
	t_Process* temp1 = process, * temp2 = process->next;
	if (temp2->next && temp1->prev)  //2 middle nodes.
	{
		temp2->next->prev = temp1;
		temp1->prev->next = temp2;
		temp1->next = temp2->next;
		temp2->prev = temp1->prev;
		temp1->prev = temp2;
		temp2->next = temp1;
		return;
	}
	else if (!temp1->prev && !temp2->next) {//only 2 nodes.
		temp2->next = temp1;
		temp1->prev = temp2;
		temp2->prev = NULL;
		temp1->next = NULL;
		headOfProcesses->process = temp2;
		tailP = temp1;
		return;
	}
	else if (!temp2->next) // two last nodes.
	{
		temp2->prev = temp1->prev;
		temp1->prev->next = temp2;
		temp1->prev = temp2;
		temp1->next = NULL;
		temp2->next = temp1;
		tailP = temp1;
		return;
	}
	else {// two first nodes.
		temp1->next = temp2->next;
		temp2->next->prev = temp1;
		temp1->prev = temp2;
		temp2->next = temp1;
		temp2->prev = NULL;
		headOfProcesses->process = temp2;
		return;
	}
	return;
}

