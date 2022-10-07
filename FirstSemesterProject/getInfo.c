#include "getInfo.h"


t_DLL* headD = NULL;
t_DLL* tailD = NULL;

t_Process* headP = NULL;
t_Process* tailP = NULL;

t_Process* getMemoryInfo(DWORD processID )
{
	t_Process* Process;
	t_DLL* DLL;
	headD = NULL;
	tailD = NULL;
	size_t numConverted;
	char str[MAX_PATH];

	HANDLE hProcess;
	PROCESS_MEMORY_COUNTERS pmc;

	// Open process in order to receive information
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID);
	if (NULL == hProcess)
	{
		LogError(strerror(GetLastError()));
		return NULL;
	}

	HMODULE hMods[1024];
	DWORD cbNeeded;
	TCHAR FoundProcessName[MAX_PATH];
	TCHAR wDllName[MAX_PATH];
	
	int i = 0;
	int numOfDLL = 0;
	// Get Process Name
	if (GetModuleFileNameEx(hProcess, 0, FoundProcessName, MAX_PATH))
	{
		
		// At this point, buffer contains the full path to the executable
		wcstombs_s(&numConverted, str, MAX_PATH, FoundProcessName, MAX_PATH);
		if (strlen(str) <= 1)
			return NULL;
	}
	else
	{
		// You better call GetLastError() here
		// Write To log
		LogError(strerror(GetLastError()));
		return NULL;
	}
	Process = (t_Process*)malloc(sizeof(t_Process));
	strcpy(Process->ProcessName, str);
	if (!Process)
	{
		LogError(strerror(GetLastError()));
		return NULL;
	}
	if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
	{
		Process->pmc.WorkingSetSize = pmc.WorkingSetSize;
		Process->pmc.PageFaultCount = pmc.PageFaultCount;
		Process->pmc.PagefileUsage = pmc.PagefileUsage;
		Process->pmc.QuotaPagedPoolUsage = pmc.QuotaPagedPoolUsage;
		Process->pmc.QuotaPeakPagedPoolUsage = pmc.QuotaPeakPagedPoolUsage;
	}


	// Get Dlls List

	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			// Get the full path to the module's file.
			if (GetModuleFileNameEx(hProcess, hMods[i], wDllName, MAX_PATH))
			{
				// * Get the module name and handle value.

				// Convert wChar to regular char array (string)
				
					wcstombs_s(&numConverted, str, MAX_PATH, wDllName, MAX_PATH);
					if (strlen(str) > 1)
					{
						numOfDLL++;
						DLL = (t_DLL*)malloc(sizeof(t_DLL));
						if (!DLL)
						{
							LogError(strerror(GetLastError()));
							return NULL;
						}
						strcpy(DLL->DLLName, str);
						addToDllList(DLL);
					}
					
				
				
			}
		}
		Process->DLL = headD;
		Process->numOfDLL = numOfDLL;
	}
	Process->ProcessID = processID;
	if (i == 0) // no dll for this process
	{
		Process->numOfDLL = 0;
		Process->DLL = NULL;
	}
	
	CloseHandle(hProcess);
	return Process;
}
t_snapShot* GetProcessesInfo(t_snapShot* oldSnapShot)
{
	// Get Processes
	t_snapShot* snapShot;
	t_Process* process;
	headP = NULL;
	tailP = NULL;
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;
	int pCounter = 0;
	// * Receive all process ID and put in aProcesses Array
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		LogError(strerror(GetLastError()));
		return NULL;
	}
	snapShot = (t_snapShot*)malloc(sizeof(t_snapShot));
	if(!snapShot)
	{
		LogError(strerror(GetLastError()));
		return NULL;
	}
	// Calculate how many process identifiers were returned.
	cProcesses = cbNeeded / sizeof(DWORD);

	// Print the memory usage for each process
	// *Loop of all processes
	

	for (i = 0; i < cProcesses; i++)
	{
		process = getMemoryInfo(aProcesses[i]);
		if (process)
		{
			pCounter++;
			addToProcessList(process);
		}
	}
	snapShot->processCounter = pCounter;
	snapShot->process = headP;
	snapShot->next = NULL;
	
	// For each Process to get its Memory Information
	if (oldSnapShot)
	{
		oldSnapShot = sumProcessesAndDLL(oldSnapShot, snapShot);
		return oldSnapShot;
	}
	return snapShot;
}

t_snapShot* sumProcessesAndDLL(t_snapShot* oldSnapShot, t_snapShot* newSnapShot)
{
	t_Process* oldTempProcessesList = oldSnapShot->process;
	t_Process* newTempProcessesList = newSnapShot->process;
	t_DLL* oldDllList = oldSnapShot->process->DLL;
	t_DLL* newDllList = newSnapShot->process->DLL;
	t_DLL* newDLL;
	t_Process* newProcess;
	while (newSnapShot->process)
	{
		newDllList = newSnapShot->process->DLL;
		oldSnapShot->process = oldTempProcessesList;
		while (oldSnapShot->process)
		{
			oldDllList = oldSnapShot->process->DLL;
			if (oldSnapShot->process->ProcessID == newSnapShot->process->ProcessID)
			{
				
				oldSnapShot->process->pmc.PageFaultCount += newSnapShot->process->pmc.PageFaultCount;
				oldSnapShot->process->pmc.PagefileUsage += newSnapShot->process->pmc.PagefileUsage;
				oldSnapShot->process->pmc.WorkingSetSize += newSnapShot->process->pmc.WorkingSetSize;
				oldSnapShot->process->pmc.QuotaPagedPoolUsage += newSnapShot->process->pmc.QuotaPagedPoolUsage;
				oldSnapShot->process->pmc.QuotaPeakPagedPoolUsage += newSnapShot->process->pmc.QuotaPeakPagedPoolUsage;
				while (newSnapShot->process->DLL)
				{
					oldSnapShot->process->DLL = oldDllList;
					while (oldSnapShot->process->DLL)
					{
						if (strcmp(oldSnapShot->process->DLL->DLLName, newSnapShot->process->DLL->DLLName) == 0)
						{
							break;
						}
						if (!oldSnapShot->process->DLL->next)
						{
							newDLL = (t_DLL*)malloc(sizeof(t_DLL));
							*newDLL = *(newSnapShot->process->DLL);
							printf("old dll list: \n\n------------\n");
							printDllList(oldDllList);
							printf("new dll is: %s\n", newSnapShot->process->DLL->DLLName);
							addNewDll(oldSnapShot->process->DLL, newDLL);
							oldSnapShot->process->numOfDLL++;
							printf("New dll list\n\n------------\n");
							printDllList(oldDllList);
							break;
						}
						oldSnapShot->process->DLL = oldSnapShot->process->DLL->next;
					}
					newSnapShot->process->DLL = newSnapShot->process->DLL->next;
				}
				break; 
			}          
			if (!oldSnapShot->process->next)
			{
				newProcess = (t_Process*)malloc(sizeof(t_Process));
				printf("List before: \n --------\n");
				printProcessList(oldTempProcessesList);
				*newProcess = *newSnapShot->process;
				printf("the new process is %s\n", newSnapShot->process->ProcessName);
				addNewProcess(oldSnapShot->process, newProcess);
				oldSnapShot->processCounter++;
				printf("List after: \n--------\n");
				printProcessList(oldTempProcessesList);
				break;
			}
			oldSnapShot->process = oldSnapShot->process->next;
		}
		oldSnapShot->process->DLL = oldDllList;
		newSnapShot->process->DLL = newDllList;
		newSnapShot->process = newSnapShot->process->next;
	}
	oldSnapShot->process = oldTempProcessesList;
	newSnapShot->process = newTempProcessesList;

	resetSnapShot(newSnapShot);
	return oldSnapShot;
}

void addToDllList(t_DLL* DLL)
{
	if (!DLL)
	{
		return;
	}
	if (!headD)
	{
		DLL->next = DLL->prev = NULL;
		headD = tailD = DLL;
	}
	else
	{
		tailD->next = DLL;
		DLL->prev = tailD;
		DLL->next = NULL;
		tailD = DLL;
	}

}

void addToProcessList(t_Process* process)
{
	if (!headP)
	{
		process->next = process->prev = NULL;
		headP = tailP = process;
	}
	else
	{
		tailP->next = process;
		process->prev = tailP;
		process->next = NULL;
		tailP = process;
	}
}

void addNewDll(t_DLL* oldDLL, t_DLL* newDLL)
{
	if (!newDLL)
	{
		return;
	}
	oldDLL->next = newDLL;
	newDLL->prev = oldDLL;
	newDLL->next = NULL;
}

void addNewProcess(t_Process* oldProcess, t_Process* newProcess)
{
	oldProcess->next = newProcess;
	newProcess->prev = oldProcess;
	newProcess->next = NULL;
}

void printDllList(t_DLL* dll)
{
	t_DLL* temp = dll;
	while (temp)
	{
		printf("%s\n", temp->DLLName);
		temp = temp->next;
	}
}


void printProcessList(t_Process* process)
{
	//t_Process* temp = process;
	while (process)
	{
		printf("%s\n", process->ProcessName);
		process = process->next;
	}

}

void freeProcess(t_Process* Process)
{
	t_DLL* curr = Process->DLL;
	while (Process->DLL)
	{
		curr = Process->DLL;
		Process->DLL = Process->DLL->next;
		free(curr);
	}
	free(Process);
}

