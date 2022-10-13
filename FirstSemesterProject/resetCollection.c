#include "resetCollection.h"
void resetSnapShot(t_snapShot* snapShotList) { // deletes one snapshot
	t_snapShot* currentSnapShot;
	t_Process* currentPorcess;
	t_DLL* currentDLL;

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
		snapShotList->process = snapShotList->process->next;
		free(currentPorcess);
	}
	free(currentSnapShot);
}

void resetCollection(t_snapShot* snapShotList, t_headerOfFile* header) { // deletes all list
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
	free(header);
	addToList(NULL);
}