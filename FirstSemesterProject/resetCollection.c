#include "resetCollection.h"
void resetProcess(t_Process* process) { // deletes one snapshot
	t_Process* tempPorcess;
	t_DLL* tempDLL;

	tempPorcess = process;
	if (!tempPorcess)
	{
		return;
	}
	tempDLL = tempPorcess->DLL;
	while (tempPorcess->DLL)
	{
		tempDLL = tempPorcess->DLL;
		tempPorcess->DLL = tempPorcess->DLL->next;
		free(tempDLL);
	}
	tempPorcess->DLL = NULL;
	free(tempPorcess);
	tempPorcess = NULL;
}

void resetCollection(t_snapShot* snapShotList, t_headerOfFile* header, Dictionary* dict) { // deletes all list
	t_snapShot* currentSnapShot;
	t_Process* currentPorcess;
	t_DLL* currentDLL;
	currentSnapShot = snapShotList;
	while (snapShotList)
	{
		currentSnapShot = snapShotList;
		while (snapShotList->process)
		{
			currentPorcess = snapShotList->process;
			while (snapShotList->process->DLL)
			{
				currentDLL = snapShotList->process->DLL;
				snapShotList->process->DLL = snapShotList->process->DLL->next;
				free(currentDLL);
			}
			snapShotList->process->DLL = NULL;
			snapShotList->process = snapShotList->process->next;
			free(currentPorcess);
		}
		snapShotList->process = NULL;
		snapShotList = snapShotList->next;
		free(currentSnapShot);
	}
	if (header)
	{
		free(header);
	}
	addToList(NULL);
	if (!dict)
	{
		return;
	}
	resetDict(dict);
}

void resetPDict(P_Dictionary* head)
{
	P_Dictionary* curr = head;
	if (!curr)
	{
		return;
	}
	while (head)
	{
		curr = head;
		head = head->next;
		free(curr);
	}
	head = NULL;
}

void resetDict(Dictionary* head)
{
	Dictionary* curr;
	t_Process* currProc;
	if (!head)
	{
		return;
	}
	while (head)
	{
		curr = head;
		currProc = curr->value;
		{
			currProc = curr->value;
			curr->value = curr->value->next;
			free(currProc);
		}
		curr->value = NULL;
		head = head->next;
	}
	head = NULL;
}