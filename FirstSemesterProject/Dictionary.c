#include "Dictionary.h"

Dictionary* dictHead = NULL, *dictTail = NULL;
P_Dictionary* P_dictHead = NULL, * P_dictTail = NULL;

Dictionary* buildDictionary(t_snapShot* head)
{
	if (!head)
	{
		LogError("Snap shot list is empty.");
		return;
	}
	t_snapShot* curr = head;
	t_Process* currProcess;
	t_DLL* currDLL;
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
	Dictionary* sub = (Dictionary*)malloc(sizeof(Dictionary));
	if (!sub)
	{
		LogError(strerror(GetLastError()));
		return NULL;
	}
	sub->value = (t_Process*)malloc(sizeof(t_Process));
	if (!sub->value)
	{
		LogError(strerror(GetLastError()));
		return NULL;
	}
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
			if (strcmp(curr->key, sub->key) == 0)
			{
				addNewValue(curr, sub->value);
				free(sub->value);
				free(sub);
				return;
			}
			if (!curr->next)
			{
				dictTail->next = sub;
				sub->prev = dictTail;
				dictTail = sub;
				return;
			}
			curr = curr->next;
		}
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
			if (!newProcess)
			{
				LogError(strerror(GetLastError()));
				return NULL;
			}
			*newProcess = *process;
			newProcess->next = NULL;
			curr->next = newProcess;
			newProcess->prev = curr;
			break;
		}
		curr = curr->next;
	}
}

P_Dictionary* buildProcessDictionary(t_snapShot* head)
{
	t_snapShot* curr = head;
	t_Process* currProcess;
	if (!head)
	{
		return NULL;
	}
	while (curr)
	{
		currProcess = curr->process;
		while (currProcess)
		{
			addProcessToDictionary(currProcess);
			currProcess = currProcess->next;
		}
		curr = curr->next;
	}
	return P_dictHead;
}
void addProcessToDictionary(t_Process* process)
{
	P_Dictionary* sub = (P_Dictionary*)malloc(sizeof(P_Dictionary));
	P_Dictionary* curr;
	if (!sub)
	{
		LogError(strerror(GetLastError()));
		return NULL;
	}
	sub->value = (t_Process*)malloc(sizeof(t_Process));
	if (!sub->value)
	{
		LogError(strerror(GetLastError()));
		return NULL;
	}
	*sub->value = *process;
	sub->next = sub->prev = NULL;
	t_Process* newProcess;
	if (!P_dictHead)
	{
		P_dictHead = P_dictTail = sub;
	}
	curr = P_dictHead;
	while (curr)
	{
		if (curr->value->ProcessID == sub->value->ProcessID)
		{
			return;
		}
		if (!curr->next)
		{
			P_dictTail->next = sub;
			sub->prev = P_dictTail;
			P_dictTail = sub;
			return;
		}
		curr = curr->next;
	}
}


int countNumOfDLL(Dictionary* head)
{
	Dictionary* curr = head;
	int count = 0;
	while (curr)
	{
		count++;
		curr = curr->next;
	}
	return count;
}

int countNumOfProcesses(P_Dictionary* head)
{
	P_Dictionary* curr = head;
	int count = 0;
	while (curr)
	{
		count++;
		curr = curr->next;
	}
	return count;
}