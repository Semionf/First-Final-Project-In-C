#include "Dictionary.h"

Dictionary* dictHead = NULL, *dictTail = NULL;


Dictionary* buildDictionary(t_snapShot* head)
{
	if (!head)
	{
		LogError("Snap shot list is empty.");
	}
	Dictionary* dict = NULL;
	t_snapShot* curr = head;
	t_Process* currProcess = head->process;
	t_DLL* currDLL = head->process->DLL;
	while (curr)
	{
		currProcess = curr->process;
		while (currProcess)
		{
			currDLL = currProcess->DLL;
			while (currDLL)
			{
				addToDictionary(currDLL->DLLName,currProcess);
				currDLL = currDLL->next;
			}
			currProcess = currProcess->next;
		}
		curr = curr->next;
	}
	return dictHead;
}

void addToDictionary(char* DLLName, t_Process* process)
{
	int i = 0;
	Dictionary* sub = (Dictionary*)malloc(sizeof(Dictionary));
	sub->value = (t_Process*)malloc(sizeof(t_Process));
	Dictionary* curr = dictHead;
	strcpy(sub->key, DLLName);
	strcpy(sub->value->ProcessName, process->ProcessName);
	sub->value->pmc = process->pmc;
	sub->next = sub->prev = NULL;
	sub->value->next = sub->value->prev = NULL;
	if (!dictHead)
	{
		dictHead = dictTail = sub;
	}
	else {
		while (curr)
		{
			if (strcmp(curr->key, DLLName) == 0)
			{
				addNewValue(curr, process);
				free(sub->value);
				free(sub);
				return;
			}
			if (!curr->next)
			{
				curr->next = sub;
				sub->prev = curr;
				return;
			}
			curr = curr->next;
		}
		dictTail->next = sub;
		sub->prev = dictTail;
		sub->next = NULL;
		dictTail = sub;
		return;
	}
}

void addNewValue(Dictionary* currDict, t_Process* process)
{
	t_Process* newProcess;
	t_Process* curr = currDict->value;
	while (curr)
	{
		if (strcmp(curr->ProcessName, process->ProcessName) == 0)
		{
			break;
		}
		if (!curr->next)
		{
			newProcess = (t_Process*)malloc(sizeof(t_Process));
			*newProcess = *process;
			newProcess->next = NULL;
			curr->next = newProcess;
			newProcess->prev = curr;
			break;
		}
		curr = curr->next;
	}
}