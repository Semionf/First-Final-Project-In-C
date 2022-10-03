#include "resetCollection.h"
void resetSnapShot(t_snapShot* snapShotList) {
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

void resetCollection(t_snapShot* snapShotList) {
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
			snapShotList->process = snapShotList->process->next;
			free(currentPorcess);
		}
		snapShotList = snapShotList->next;
		free(currentSnapShot);
	}
}