#include "generateHtmlReport.h"

void generateHtmlReport(t_snapShot* head, Dictionary* dictHead)
{
	int i = 1;
	char fileName[20];
	t_snapShot* currS = head;
	t_Process* currP;
	Dictionary* currD = dictHead;
	homePage(currS,currD);
	while (currS)
	{
		sprintf(fileName, "C:\\html\\sample%d.html", i);
		generateSnapShotPage(currS,fileName);
		i++;
		currS = currS->next;
	}
	i = 0;
	while (currD)
	{
		sprintf(fileName, "C:\\html\\dll%d.html", i);
		generateDLLPage(currD, fileName);
		i++;
		currD = currD->next;
	}
}

void homePage(t_snapShot* headS, Dictionary* headD)
{
	char htmlHomePage[5000];
	char aboutMe[200];
	int num = 1;
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
	FILE* f = fopen("C:\\html\\home.html", "w");
	if (!f)
	{
		exit(1);
	}				 
	strcpy(aboutMe, "file:///C:/html/aboutMe.html");
	sprintf(htmlHomePage, "<!DOCTYPE html><html lang =\"en\"><head><meta charset =\"UTF-8\"><meta http - equiv =\"X-UA-Compatible\" content =\"IE=edge\"><meta name =\"viewport\" content =\"width=device-width, initial-scale=1.0\"><link rel =\"stylesheet\" href =\"style.css\"><link rel =\"stylesheet\" href =\"dll.html\">\n<title>Document</title></head><body><header><div class =\"headerDiv\">\n<nav><ul class=\"Nav\"><img class=\"logoImg\" src=\"zionet.png\" alt=\"\"><li><input type=\"text\" placeholder=\"DLL Count: %d\"></li><li><input type=\"text\" placeholder = \"Process Count: %d\"></li><li><input type=\"text\" placeholder=\"Memory Avg: %d\"></li><li><div class=\"box1\"><h1>Welcome to Semion's Process Manager Website!</h1></div></li><li><a href=\"%s\"><img class=\"aboutMe\" src=\"240_F_305482908_bWpX5j1m1cum5tiZ42Y5J2RyOQAba3c1.jpg\" alt=\"\"></a></li></ul></nav></div></header><main><div class=\"HeadLines\"><div><h1>SAMPLES LIST</h1></div><div><h1>DLL'S LIST</h1></div></div><div class=\"tables\"><div class=\"column-left\"><table class=\"leftTable\"><tr><th>ID</th><th>Snapshot List</th><th>DLL count</th><th>Memory Avg</th></tr>",numOfDLL, numOfProcess, avgOfWorkingSetSize,aboutMe);
	fputs(htmlHomePage,f);
	while (currS)
	{
		fputs(SamplesList(currS,currD),f);
		currS = currS->next;
	}
	sprintf(htmlHomePage, "</table></div><div class=\"column-right\"><table class=\"rightTable\"><tr><th class=\"column-right-td\">DLL Name</th><th class=\"column-right-td\">Link</th></tr>");
	fputs(htmlHomePage, f);
	currD = headD;
	while (currD)
	{
		fputs(DLLList(currD, num), f);
		num++;
		currD = currD->next;
	}
	sprintf(htmlHomePage, " </table></div></div></main></body></html>");
	fputs(htmlHomePage, f);
	fclose(f);
}

void generateSnapShotPage(t_snapShot* curr, char* fileName)
{
	t_snapShot* snapShot = curr;
	t_Process* currProcess;
	t_DLL* currDLL;
	char htmlSnapShotPage[5000];
	char lineInTable[1000];
	char* str;
	FILE* f = fopen(fileName, "w");
	if (!f)
	{
		exit(1);
	}
	sprintf(htmlSnapShotPage, "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta http - equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Document</title><link rel=\"stylesheet\" href=\"style.css\"></head><body><div class=\"headerDiv\"><nav><ul class =\"Nav\"><img class =\"logoImg\" src=\"zionet.png\" alt=""><li><div class=\"box1\"><h1>Sample List Number : %d At %s </h1></div></li><li><a href=\"file:///C:/html/home.html\"><img src=\"25694.png\" alt=""></a></li><li><a href=\"file:///C:/html/aboutMe.html\"><img class=\"aboutMe\" src=\"240_F_305482908_bWpX5j1m1cum5tiZ42Y5J2RyOQAba3c1.jpg\" alt=""></a></li></ul></nav></div><div class=\"sample\"><table><tr><th>Process Name</th><th>Process ID</th><th>PageFaultCount</th><th>WorkingSetSize</th><th>QuotaPagedPoolUsage</th><th>QuotaPeakPagedPoolUsage</th><th>PagefileUsage</th><th>DLL Count</th><th>Process\'s DLL List</th></tr>",snapShot->sampleNumber, snapShot->timeOfSample);
	fputs(htmlSnapShotPage, f);
	currProcess = snapShot->process;
	while (currProcess)
	{
		currDLL = currProcess->DLL;
		sprintf(lineInTable, " <td>%s</td><td>%d</td><td>%ld</td><td>%ld</td><td>%ld</td><td>%llu</td><td>%ld</td><td>%d</td>", currProcess->ProcessName, currProcess->ProcessID, currProcess->pmc.PageFaultCount, currProcess->pmc.WorkingSetSize, currProcess->pmc.QuotaPagedPoolUsage, currProcess->pmc.QuotaPeakNonPagedPoolUsage, currProcess->pmc.PagefileUsage, currProcess->numOfDLL);
		fputs(lineInTable, f);
		sprintf(lineInTable, "<td class=\"wrapper\"><div class=\"selectDLL\"><select class=\"selectDLL\"><option>List</option><option>C:\\Windows\\SYSTEM32\\ntdll.dll</option>");
		fputs(lineInTable, f);
		while (currDLL)
		{
			str = addOption(currDLL);
			fputs(str,f);
			currDLL = currDLL->next;
		}
		currProcess = currProcess->next;
	}
	sprintf(lineInTable, "</select></div></td></tr></table></head></div><body></body></html>");
	fputs(lineInTable, f);
	fclose(f);
}

void generateDLLPage(Dictionary* curr, char* fileName)
{
	char htmlHomePage[5000];
	char aboutMe[200];
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

long calculateAvgWorkingSetSizeForCurrentSnapShot(t_snapShot* snapShot)
{
	t_snapShot* curr = snapShot;
	if (!curr)
	{
		return NULL;
	}
	t_Process* currProcess;
	long long sum = 0;
	long avg;
	currProcess = curr->process;
	while (currProcess)
	{
		sum += currProcess->pmc.WorkingSetSize;
		currProcess = currProcess->next;
	}
	curr = curr->next;

	avg = sum / snapShot->processCounter;
	return avg;
}
char* SamplesList(t_snapShot* snapShot, Dictionary* dict)
{
	t_snapShot* curr = snapShot;
	Dictionary* currD = dict;
	char str[1000];

	char fileAddress[40];
	int numOfDLL = countNumOfDLL(currD);
	long avgWorkingSetSize;
	if (!curr)
	{
		return;
	}
	avgWorkingSetSize = calculateAvgWorkingSetSizeForCurrentSnapShot(curr);
	sprintf(fileAddress, "file:///C:/html/sample%d.html", curr->sampleNumber);
	sprintf(str, "<tr><td>%d</td><td><a href=\"%s\">sample%d.html</a></td><td>%d</td><td>%ld</td></tr>", curr->sampleNumber, fileAddress, curr->sampleNumber, numOfDLL, avgWorkingSetSize);

	return str;
}

char* DLLList(Dictionary* currDict, int num)
{
	char str[1000];
	char fileAddress[40];
	Dictionary* curr = currDict;
	if (!curr)
	{
		return;
	}
	sprintf(fileAddress, "file:///C:/html/dll%d.html", num);
	sprintf(str, "<tr><td class=\"rightTable\"><a href=\"%s\">%s</a></td><td class=\"tdMainTable\">dll%d.html</td></tr>", fileAddress, curr->key, num);

	return str;
}

char* addOption(t_DLL* currDLL)
{
	char str[1000];
	sprintf(str, "<option>%s</option>", currDLL->DLLName);
	return str;
}