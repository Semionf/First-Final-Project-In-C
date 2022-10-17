#include "Dictionary.h"

Dictionary* dictHead = NULL, *dictTail = NULL;
P_Dictionary* P_dictHead = NULL, * P_dictTail = NULL;

Dictionary* buildDictionary(t_snapShot* head) //build dictionary
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

void addToDictionary(char* DLLName, t_Process* process) //	add key
{
	char temp1[MAX_PATH], temp2[MAX_PATH];
	Dictionary* sub = (Dictionary*)malloc(sizeof(Dictionary));
	if (!sub)
	{
		LogError(strerror(GetLastError()));
		return NULL;
	}
	sub->ProcessesCount = 1;
	sub->value = (t_Process*)malloc(sizeof(t_Process));
	if (!sub->value)
	{
		LogError(strerror(GetLastError()));
		return NULL;
	}
	Dictionary* curr = dictHead;
	*sub->value = *process;
	strcpy(sub->key, DLLName);
	sub->next = sub->prev = NULL;
	sub->value->next = sub->value->prev = NULL;
	if (!dictHead)
	{
		dictHead = dictTail = sub;
	}
	else {
		strcpy(temp2, sub->key);
		strcpy(temp2, toLower(temp2));
		while (curr)
		{
			strcpy(temp1, curr->key);
			strcpy(temp1, toLower(temp1));
			if (strcmp(temp1, temp2) == 0)
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

void addNewValue(Dictionary* currDict, t_Process* process) // add value
{
	t_Process* newProcess = process;
	t_Process* curr = currDict->value;
	while (curr)
	{
		if (curr->ProcessID == newProcess->ProcessID)
		{
			break;
		}
		if (!curr->next)
		{
			currDict->ProcessesCount++;
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

P_Dictionary* buildProcessDictionary(t_snapShot* head) // building process dictionary
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
void addProcessToDictionary(t_Process* process) // add key
{
	P_Dictionary* sub = (P_Dictionary*)malloc(sizeof(P_Dictionary));
	P_Dictionary* curr;
	if (!sub)
	{
		LogError(strerror(GetLastError()));
		return NULL;
	}
	strcpy(sub->value, process->ProcessName);
	sub->ProcessID = process->ProcessID;
	sub->next = sub->prev = NULL;
	if (!P_dictHead)
	{
		P_dictHead = P_dictTail = sub;
	}
	curr = P_dictHead;
	while (curr)
	{
		if (curr->ProcessID == sub->ProcessID && (strcmp(sub->value, curr->value)==0))
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


int countNumOfDLL(Dictionary* head) // counting number of DLL in dictionary
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

int countNumOfProcesses(P_Dictionary* head) // counting number of Processes in dictionary
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

char* toLower(char* str) // making unique DLL keys (some of DLL names are similar, diffrence is Capital letter or so)
{
	for (char* p = str; *p; p++)
	{
		*p = tolower(*p);
	}
	return str;
}