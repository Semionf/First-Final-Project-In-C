#include "generateHtmlReport.h"

void generateHtmlReport(t_snapShot* head, Dictionary* dictHead)
{
	t_snapShot* currS = head;
	Dictionary* currD = dictHead;
	homePage(currS,currD);
	while (currS)
	{
		generateSnapShotPage(currS);
		currS = currS->next;
	}
	while (currD)
	{
		generateDLLPage(currD);
		currD = currD->next;
	}
}

void homePage(t_snapShot* headS, Dictionary* headD)
{
	int totalDLL;
	long avgOfWorkingSetSize;
	Dictionary* currD = headD;
	t_snapShot* currS = headS;
	t_Process* currP = currS->process;
	totalDLL = countNumOfDLL(currP);
	avgOfWorkingSetSize = calculateAvgWorkingSetSize(currS);
}

void generateSnapShotPage(t_snapShot* curr)
{

}

void generateDLLPage(Dictionary* curr)
{

}

int countNumOfDLL(t_Process* headP)
{
	int numOfDLL = 0;
	t_Process* curr = headP;
	while (curr)
	{
		numOfDLL += curr->numOfDLL;
		curr = curr->next;
	}
	return numOfDLL;
}

long calculateAvgWorkingSetSize(t_snapShot* snapShot)
{
	t_Process* curr = snapShot->process;
	long sum = 0;
	long avg;
	while (curr)
	{
		sum += curr->pmc.WorkingSetSize;
		curr = curr->next;
	}
	avg = sum / snapShot->processCounter;
	return avg;
}