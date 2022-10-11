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
	char htmlHomePage[10000];
	int numOfDLL;
	int numOfProcess;
	long avgOfWorkingSetSize;
	Dictionary* currD = headD;
	t_snapShot* currS = headS;
	t_Process* currP = currS->process;
	P_Dictionary* head = buildProcessDictionary(currS);
	numOfDLL = countNumOfDLL(currD);
	numOfProcess = countNumOfProcesses(head);
	avgOfWorkingSetSize = calculateAvgWorkingSetSize(currS);
}

void generateSnapShotPage(t_snapShot* curr)
{

}

void generateDLLPage(Dictionary* curr)
{

}


long calculateAvgWorkingSetSize(t_snapShot* snapShot)
{
	t_snapShot* curr = snapShot;
	if (!curr)
	{
		return NULL;
	}
	t_Process* currProcess;
	long long sum = 0;
	long avg;
	while (curr)
	{
		currProcess = curr->process;
		while (currProcess)
		{
			sum += currProcess->pmc.WorkingSetSize;
			currProcess = currProcess->next;
		}
		curr = curr->next;
	}
	avg = sum / snapShot->processCounter;
	return avg;
}
