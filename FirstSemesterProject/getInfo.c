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
		LogError(strerror(GetLastError()));
		return NULL;
	}
	Process = (t_Process*)malloc(sizeof(t_Process));
	if (!Process)
	{
		LogError(strerror(GetLastError()));
		return NULL;
	}
	strcpy(Process->ProcessName, str); 
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

t_snapShot* sumProcessesAndDLL(t_snapShot* oldSnapShot, t_snapShot* newSnapShot) // this function sum between snapshots
{
	t_Process* oldTempProcessesList = oldSnapShot->process;
	t_Process* newTempProcessesList = newSnapShot->process;
	t_DLL* oldDllList;
	t_DLL* newDllList;
	t_DLL* newDLL;
	t_Process* newProcess;
	while (newTempProcessesList)
	{
		oldTempProcessesList = oldSnapShot->process;
		while (oldTempProcessesList)
		{
			oldDllList = oldTempProcessesList->DLL;
			if (oldTempProcessesList->ProcessID == newTempProcessesList->ProcessID) // if processes got the same id, info is summed up
			{
				
				oldTempProcessesList->pmc.PageFaultCount += newTempProcessesList->pmc.PageFaultCount;
				oldTempProcessesList->pmc.PagefileUsage += newTempProcessesList->pmc.PagefileUsage;
				oldTempProcessesList->pmc.WorkingSetSize += newTempProcessesList->pmc.WorkingSetSize;
				oldTempProcessesList->pmc.QuotaPagedPoolUsage += newTempProcessesList->pmc.QuotaPagedPoolUsage;
				oldTempProcessesList->pmc.QuotaPeakPagedPoolUsage += newTempProcessesList->pmc.QuotaPeakPagedPoolUsage;
				newDllList = newTempProcessesList->DLL;
				while (newDllList)
				{
					oldDllList = oldTempProcessesList->DLL;
					while (oldDllList)
					{
						if (strcmp(oldDllList->DLLName, newDllList->DLLName) == 0)
						{
							break;
						}
						if (!oldDllList->next) // adding new DLL
						{
							newDLL = (t_DLL*)malloc(sizeof(t_DLL));
							if (!newDLL)
							{
								LogError(strerror(GetLastError()));
								return NULL;
							}
							strcpy(newDLL, newDllList);
							printf("old dll list: \n\n------------\n");
							printDllList(oldTempProcessesList->DLL);
							printf("new dll is: %s\n", newDLL->DLLName);
							addNewDll(oldDllList, newDLL);
							oldTempProcessesList->numOfDLL++;
							printf("New dll list\n\n------------\n");
							printDllList(oldTempProcessesList->DLL);
							break;
						}
						oldDllList = oldDllList->next;
					}
					oldDllList = oldTempProcessesList->DLL;
					newDllList = newDllList->next;
				}
				break; 
			}          
			if (!oldTempProcessesList->next) // adding new Process
			{
				newProcess = (t_Process*)malloc(sizeof(t_Process));
				if (!newProcess)
				{
					LogError(strerror(GetLastError()));
					return NULL;
				}
				//printf("List before: \n --------\n");
				//printProcessList(oldTempProcessesList);
				newProcess->DLL = newTempProcessesList->DLL;
				newProcess->numOfDLL = newTempProcessesList->numOfDLL;
				newProcess->pmc = newTempProcessesList->pmc;
				newProcess->ProcessID = newTempProcessesList->ProcessID;
				strcpy(newProcess->ProcessName, newTempProcessesList->ProcessName);
				//printf("the new process is %s\n", newSnapShot->process->ProcessName);
				addNewProcess(oldTempProcessesList, newProcess);
				oldSnapShot->processCounter++;
				//printf("List after: \n--------\n");
				//printProcessList(oldTempProcessesList);
				break;
			}
			oldTempProcessesList = oldTempProcessesList->next;
		}
		newTempProcessesList = newTempProcessesList->next;
	}
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

void printDllList(t_DLL* dll) // a function that i was checking if dll has been added
{
	t_DLL* temp = dll;
	while (temp)
	{
		printf("%s\n", temp->DLLName);
		temp = temp->next;
	}
}


void printProcessList(t_Process* process) // a function that i was checking if process has been added
{
	//t_Process* temp = process;
	while (process)
	{
		printf("%s\n", process->ProcessName);
		process = process->next;
	}

}


