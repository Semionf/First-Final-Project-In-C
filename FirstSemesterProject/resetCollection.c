#include "resetCollection.h"
void resetCollection(t_snapShot* snapShotList) {
	t_snapShot* currentSnapShot = snapShotList;
	t_Process* currentPorcess;
	t_DLL* currentDLL;

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
			snapShotList->process = snapShotList->process->next;
			free(currentPorcess);
		}
		snapShotList = snapShotList->next;
		free(currentSnapShot);
	}
	addToList(NULL);
}