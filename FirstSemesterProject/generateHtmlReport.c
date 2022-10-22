#include "generateHtmlReport.h"

void generateHtmlReport(t_snapShot* head, Dictionary* dictHead) 
{
	if (!head)
	{
		return;
	}
	int i = 1;
	char fileName[25];
	t_snapShot* currS = head;
	Dictionary* currD = dictHead;
	homePage(currS,currD);

	while (currS)
	{
		sprintf(fileName, "C:\\html\\sample%d.html", i);
		generateSnapShotPage(currS,fileName);
		i++;
		currS = currS->next;
	}
	i = 1;
	while (currD)
	{
		sprintf(fileName, "C:\\html\\dll%d.html", i);
		generateDLLPage(currD, fileName);
		i++;
		currD = currD->next;
	}
	generateAboutMePage();
}

void homePage(t_snapShot* headS, Dictionary* headD) //making home page
{
	char htmlHomePage[5000];
	char aboutMe[200];
	int num = 1;
	int numOfDLL;
	int numOfProcess;
	long long avgOfWorkingSetSize;
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
		LogError(strerror(GetLastError()));
		exit(1);
	}				 
	strcpy(aboutMe, "file:///C:/html/aboutMe.html");
	sprintf(htmlHomePage, "<!DOCTYPE html><html lang =\"en\"><head><meta charset =\"UTF-8\"><meta http - equiv =\"X-UA-Compatible\" content =\"IE=edge\"><meta name =\"viewport\" content =\"width=device-width, initial-scale=1.0\"><link rel =\"stylesheet\" href =\"CSS/style1.css\"><link rel =\"stylesheet\" href =\"dll.html\">\n<title>HomePage</title></head><body><header><div class =\"headerDiv\">\n<nav><ul class=\"Nav\"><img class=\"logoImg\" src=\"zionet.png\" alt=\"\"><li><input type=\"text\" placeholder=\"DLL Count: %d\"><input type=\"text\" placeholder = \"Process Count: %d\"><input type=\"text\" placeholder=\"Memory Avg: %llu\"></li><li><div class=\"box1\"><h1>Welcome to Semion's Process Manager Website!</h1></div></li><li><a href=\"%s\"><img class=\"aboutMe\" src=\"240_F_305482908_bWpX5j1m1cum5tiZ42Y5J2RyOQAba3c1.jpg\" alt=\"\"></a></li></ul></nav></div></header><div class=\"tableCont\"><div class=\"divTable\"><div><h1>SAMPLES LIST</h1></div><table class=\"leftTable\"><tr><th>ID</th><th>Snapshot List</th><th>DLL count</th><th>Memory Avg</th></tr>",numOfDLL, numOfProcess, avgOfWorkingSetSize,aboutMe);
	fputs(htmlHomePage,f);
	while (currS) //each sample in table
	{
		fputs(SamplesList(currS,currD),f);
		currS = currS->next;
	}
	sprintf(htmlHomePage, "</table></div><div class=\"divTable\"><table class=\"rightTable\"><div><h1>DLL'S LIST</h1></div><tr><th>DLL Name</th><th>Link</th></tr>");
	fputs(htmlHomePage, f);
	currD = headD;
	while (currD) //each dll in table
	{
		fputs(DLLList(currD, num), f);
		num++;
		currD = currD->next;
	}
	sprintf(htmlHomePage, " </table></div></div></main></body></html>");
	fputs(htmlHomePage, f);
	fclose(f);
	resetPDict(head);
}

void generateSnapShotPage(t_snapShot* curr, char* fileName) // generating snapshot pages
{
	int index = 0;
	int highestMemIndex = highestMemoryIndex(curr);
	t_snapShot* snapShot = curr;
	t_Process* currProcess;
	t_DLL* currDLL;
	char htmlSnapShotPage[5000];
	char lineInTable[7000];
	FILE* f = fopen(fileName, "w");

	if (!f)
	{
		LogError(strerror(GetLastError()));
		exit(1);
	}
	sprintf(htmlSnapShotPage, "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta http - equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Document</title><link rel=\"stylesheet\" href=\"CSS/style2.css\"></head><body><div class=\"headerDiv\"><nav><ul class =\"Nav\"><img class =\"logoImg\" src=\"zionet.png\" alt=""><li><div class=\"box1\"><h1>Sample List Number : %d At %s </h1></div></li><li><a href=\"file:///C:/html/home.html\"><img src=\"25694.png\" alt=""></a></li><li><a href=\"file:///C:/html/aboutMe.html\"><img class=\"aboutMe\" src=\"240_F_305482908_bWpX5j1m1cum5tiZ42Y5J2RyOQAba3c1.jpg\" alt=""></a></li></ul></nav></div><div class=\"sample\"><table><tr><th>Process Name</th><th>Process ID</th><th>PageFaultCount</th><th>WorkingSetSize</th><th>QuotaPagedPoolUsage</th><th>QuotaPeakPagedPoolUsage</th><th>PagefileUsage</th><th>DLL Count</th><th>Process\'s DLL List</th></tr>",snapShot->sampleNumber, snapShot->timeOfSample);
	fputs(htmlSnapShotPage, f);
	currProcess = snapShot->process;
	while (currProcess)
	{
		currDLL = currProcess->DLL;
		if (index != highestMemIndex)
		{
			sprintf(lineInTable, "<tr><td>%s</td><td>%d</td><td>%ld</td><td>%llu</td><td>%llu</td><td>%llu</td><td>%llu</td><td>%d</td><td class=\"wrapper\"><div class=\"selectDLL\"><select class=\"selectDLL\"><option>List</option>", currProcess->ProcessName, currProcess->ProcessID, currProcess->pmc.PageFaultCount, currProcess->pmc.WorkingSetSize, currProcess->pmc.QuotaPagedPoolUsage, currProcess->pmc.QuotaPeakPagedPoolUsage, currProcess->pmc.PagefileUsage, currProcess->numOfDLL);
		}
		else
		{
			sprintf(lineInTable, "<tr><td>%s</td><td>%d</td><td>%ld</td><td>%llu<img class=\"warningImg\" src=\"4201973.png\" alt=\"\"></td><td>%llu</td><td>%llu</td><td>%llu</td><td>%d</td><td class=\"wrapper\"><div class=\"selectDLL\"><select class=\"selectDLL\"><option>List</option>", currProcess->ProcessName, currProcess->ProcessID, currProcess->pmc.PageFaultCount, currProcess->pmc.WorkingSetSize, currProcess->pmc.QuotaPagedPoolUsage, currProcess->pmc.QuotaPeakPagedPoolUsage, currProcess->pmc.PagefileUsage, currProcess->numOfDLL);
		}
		fputs(lineInTable, f);
		sprintf(lineInTable, "");
		while (currDLL)
		{
			fputs(addOption(currDLL),f);
			currDLL = currDLL->next;
		}
		index++;
		sprintf(lineInTable, "</select></tr>");
		fputs(lineInTable, f);
		currProcess = currProcess->next;
	}
	sprintf(lineInTable, "</div></td></tr></table></head></div><body></body></html>");
	fputs(lineInTable, f);
	fclose(f);
}

void generateDLLPage(Dictionary* curr, char* fileName) // generating DLL pages
{
	t_Process* currProcess = curr->value;
	char htmlDLLPage[5000];
	char lineInTable[5000];
	FILE* f = fopen(fileName, "w");
	if (!f)
	{
		LogError(strerror(GetLastError()));
		exit(1);
	}
	sprintf(htmlDLLPage, "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta http - equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Document</title><link rel=\"stylesheet\" href=\"CSS/style2.css\"></head><body><div class=\"headerDiv\"><nav><ul class=\"Nav\"><img class=\"logoImg\" src=\"zionet.png\" alt=\"\"><li><div class=\"box1\"><h1>DLL Summary Page</h1></div></li><li><a href=\"file:///C:/html/home.html\"><img src=\"25694.png\" alt=\"\"></a></li><li><a href=\"file:///C:/html/aboutMe.html\"><img class=\"aboutMe\" src=\"240_F_305482908_bWpX5j1m1cum5tiZ42Y5J2RyOQAba3c1.jpg\" alt=\"\"></a></li></ul></nav></div><div class=\"sample\"><table><tr><th>Process Name</th><th>Process ID</th><th>PageFaultCount</th><th>WorkingSetSize</th><th>QuotaPagedPoolUsage</th><th>QuotaPeakPagedPoolUsage</th><th>PagefileUsage</th></tr>");
	fputs(htmlDLLPage,f);
	while (currProcess)
	{
		sprintf(lineInTable, "<tr><th>%s</th><th>%d</th><th>%ld</th><th>%llu</th><th>%llu</th><th>%llu</th><th>%llu</th></tr>",currProcess->ProcessName,currProcess->ProcessID, currProcess->pmc.PageFaultCount, currProcess->pmc.WorkingSetSize, currProcess->pmc.QuotaPagedPoolUsage, currProcess->pmc.QuotaPeakPagedPoolUsage, currProcess->pmc.PagefileUsage);
		fputs(lineInTable, f);
		currProcess = currProcess->next;
	}
	sprintf(htmlDLLPage, "</table><div></body></html>");
	fclose(f);
}

void generateAboutMePage() // generating About me page
{
	char htmlAboutMePage[5000];
	FILE* f = fopen("C:\\html\\aboutMe.html", "w");
	if (!f)
	{
		LogError(strerror(GetLastError()));
		exit(1);
	}
	sprintf(htmlAboutMePage,"<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta http - equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name = \"viewport\" content = \"width=device-width, initial-scale=1.0\"><title>Document</title><link rel=\"stylesheet\" href=\"CSS/style3.css\"></head><body><div><nav><ul class=\"Nav\"><img class=\"logoImg\" src=\"zionet.png\" alt=\"\"><li><div class=\"box1\"><h1>About Me</h1></div></li><li><a href=\"file:///C:/html/home.html\"><img src=\"25694.png\" alt=\"\"></a></li><li class=\"github\"><a href=\"https://github.com/Semionf\"><img src=\"2175377.png\" alt=\"\"></a></li></ul></nav></div><div><div class=\"aboutMeText\"><h1>My name is Semion Furlender I\"m 32 years old, I love coding, listening to music, watching movies and serieses, and playing the guitar.</h1></div><img class=\"guitarImg\" src=\"guitar.png\" alt = \"\"><img class=\"computerImg\" src=\"computer.png\" alt=\"\"><img class=\"musicImg\" src=\"music.png\" alt=\"\"><img class=\"moviesImg\" src=\"movies.png\" alt=\"\"></div><footer><iframe class=\"video\" src=\"https://www.youtube.com/embed/tgbNymZ7vqY?controls=0\"></iframe></footer></body></html>");
	fputs(htmlAboutMePage, f);
	fclose(f);
}

long long calculateAvgWorkingSetSize(t_snapShot* snapShot) // calculating AVG of Working set size of all snapshots.
{
	t_snapShot* curr = snapShot;
	int numOfProcesses = 0;
	if (!curr)
	{
		return 0;
	}
	t_Process* currProcess;
	long long sum = 0;
	long long avg;
	while (curr)
	{
		currProcess = curr->process;
		while (currProcess)
		{
			sum += currProcess->pmc.WorkingSetSize;
			currProcess = currProcess->next;
		}
		numOfProcesses += curr->processCounter;
		curr = curr->next;
	}
	avg = sum / numOfProcesses;
	return avg;
}

long long calculateAvgWorkingSetSizeForCurrentSnapShot(t_snapShot* snapShot) // calculating AVG of Working set size for one snapshot.
{
	t_snapShot* curr = snapShot;
	if (!curr)
	{
		return 0;
	}
	t_Process* currProcess;
	long long sum = 0;
	long long avg;
	currProcess = curr->process;
	while (currProcess)
	{
		sum += currProcess->pmc.WorkingSetSize;
		currProcess = currProcess->next;
	}
	avg = sum / curr->processCounter;
	return avg;
}
char* SamplesList(t_snapShot* snapShot, Dictionary* dict) // each sample in table
{
	t_snapShot* curr = snapShot;
	t_Process* currProcess = curr->process;
	Dictionary* currD = dict;
	char str[1000];
	char fileAddress[40];
	int numOfDLL = 0;
	long long avgWorkingSetSize;
	if (!curr)
	{
		return NULL;
	}
	while (currProcess)
	{
		numOfDLL += currProcess->numOfDLL;
		currProcess = currProcess->next;
	}
	avgWorkingSetSize = calculateAvgWorkingSetSizeForCurrentSnapShot(curr);
	sprintf(fileAddress, "file:///C:/html/sample%d.html", curr->sampleNumber);
	sprintf(str, "<tr><td>%d</td><td><a href=\"%s\">sample%d.html</a></td><td>%d</td><td>%llu</td></tr>", curr->sampleNumber, fileAddress, curr->sampleNumber, numOfDLL, avgWorkingSetSize);

	return str;
}

char* DLLList(Dictionary* currDict, int num) // each dll in table
{
	char str[1000];
	char fileAddress[40];
	Dictionary* curr = currDict;
	if (!curr)
	{
		return NULL;
	}
	sprintf(fileAddress, "file:///C:/html/dll%d.html", num);
	sprintf(str, "<tr><td class=\"longName\">%s</td><td><a href=\"%s\">dll%d.html</a></td></tr>", curr->key, fileAddress, num);

	return str;
}

char* addOption(t_DLL* currDLL) // each option in the select tag
{
	char str[1000];
	sprintf(str, "<option>%s</option>", currDLL->DLLName);
	return str;
}

int highestMemoryIndex(t_snapShot* head) // this function finds the index of higest Working set size
{
	t_snapShot* temp = head;
	t_Process* tempProc;
	unsigned long long memoryConsume;
	int index = 0;
	int highestMemoryIndex = 0;
	memoryConsume = temp->process->pmc.WorkingSetSize;
	tempProc = temp->process;		
	while (tempProc)
	{
		if (memoryConsume < tempProc->pmc.WorkingSetSize)
		{
			memoryConsume = tempProc->pmc.WorkingSetSize;
			highestMemoryIndex = index;
		}
		index++;
		tempProc = tempProc->next;
	}
	return highestMemoryIndex;
}