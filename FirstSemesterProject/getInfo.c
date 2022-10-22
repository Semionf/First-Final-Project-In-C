#include "getInfo.h"


t_DLL* headD = NULL;
t_DLL* tailD = NULL;

t_Process* headP = NULL;
t_Process* tailP = NULL;

t_Process* getMemoryInfo(DWORD processID)
{
	t_Process* Process;
	t_DLL* DLL;
	size_t numConverted;
	char str[MAX_PATH];
	char str2[MAX_PATH];
	HANDLE hProcess;
	PROCESS_MEMORY_COUNTERS pmc;
	headD = NULL;
	tailD = NULL;
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
		if (!(strlen(str)  > 0))
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

				wcstombs_s(&numConverted, str2, MAX_PATH, wDllName, MAX_PATH);
				if ((strstr(str2,".dll")) || (strstr(str2, ".DLL")))
				{

					DLL = (t_DLL*)malloc(sizeof(t_DLL));
					if (!DLL)
					{
						LogError(strerror(GetLastError()));
						return NULL;
					}
					strcpy(DLL->DLLName, str2);
					addToDllList(DLL);
					numOfDLL++;
				}

			}
		}
	}
	Process->DLL = headD;
	Process->numOfDLL = numOfDLL;
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
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	int pCounter = 0;
	// * Receive all process ID and put in aProcesses Array
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		LogError(strerror(GetLastError()));
		return NULL;
	}
	snapShot = (t_snapShot*)malloc(sizeof(t_snapShot));
	if (!snapShot)
	{
		LogError(strerror(GetLastError()));
		return NULL;
	}
	// Calculate how many process identifiers were returned.
	cProcesses = cbNeeded / sizeof(DWORD);

	// Print the memory usage for each process
	// *Loop of all processes


	for (int i = 0; i < cProcesses; i++)
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
	snapShot->next = snapShot->prev = NULL;
	// For each Process to get its Memory Information
	headP = NULL;
	tailP = NULL;
	if (oldSnapShot)
	{
		sumProcessesAndDLL(oldSnapShot, snapShot);
		return oldSnapShot;
	}
	return snapShot;
}

void sumProcessesAndDLL(t_snapShot* oldSnapShot, t_snapShot* newSnapShot) // this function sum between snapshots
{
	t_Process* currOldProc = oldSnapShot->process;
	t_Process* currNewProc = newSnapShot->process;
	
	t_DLL* currOldDll;
	t_DLL* currNewDll;
	
	t_Process* newProcess;
	t_DLL* newDll;

	while (currNewProc)
	{
		currOldProc = oldSnapShot->process;
		while (currOldProc)
		{
			if (currNewProc->ProcessID == currOldProc->ProcessID)
			{
				currOldProc->pmc.PageFaultCount += currNewProc->pmc.PageFaultCount;
				currOldProc->pmc.PagefileUsage += currNewProc->pmc.PagefileUsage;
				currOldProc->pmc.QuotaPagedPoolUsage += currNewProc->pmc.QuotaPagedPoolUsage;
				currOldProc->pmc.QuotaPeakPagedPoolUsage += currNewProc->pmc.QuotaPeakPagedPoolUsage;
				currOldProc->pmc.WorkingSetSize += currNewProc->pmc.WorkingSetSize;
				currNewDll = currNewProc->DLL;
				while (currNewDll)
				{
					currOldDll = currOldProc->DLL;
					while (currOldDll)
					{
						if (strcmp(currOldDll->DLLName, currNewDll->DLLName) == 0)
						{
							break;
						}
						else if(!currOldDll->next)
						{
							newDll = (t_DLL*)malloc(sizeof(t_DLL));
							if (!newDll)
							{
								LogError(strerror(GetLastError()));
								return NULL;
							}
							*newDll = *currNewDll;
							addNewDll(currOldDll, newDll);
							currOldProc->numOfDLL++;
						}
						currOldDll = currOldDll->next;
					}
					currNewDll = currNewDll->next;
				}
				break;
			}
			else if (!currOldProc->next)
			{
				newProcess = (t_Process*)malloc(sizeof(t_Process));
				if (!newProcess)
				{
					LogError(strerror(GetLastError()));
					return NULL;
				}
				*newProcess = *currNewProc;
				addNewProcess(currOldProc, newProcess);
				oldSnapShot->processCounter++;
				break;
			}
			currOldProc = currOldProc->next;
		}
		currNewProc = currNewProc->next;
	}
	resetCollection(newSnapShot,NULL,NULL);
	printProcessList(oldSnapShot->process);
}
/*

t_snapShot* GetProcessesInfo(t_snapShot* prevSnapShot)
{
	// Get Processes

	// Receive all processes ID
	DWORD aProcesses[1024], cbNeeded, cProcesses;  //DWORD aProcesses[1024] -> An array into which the process IDs will be entered
	unsigned int i;

	// *Receive all process ID and put in aProcesses Array
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))  //The function inserts into the array the entire list of processes running in the system
	{
		LogError("There are no processes in the system");

		return NULL;
	}

	// Calculate how many process identifiers were returned.
	cProcesses = cbNeeded / sizeof(DWORD);

	if (prevSnapShot == NULL)
	{
		headP = NULL;
		tailP = NULL;
		int processCount = 0;


		//Defining a pointer variable of type struct SnapShot in order to enter the data into it
		t_snapShot* mySnapShot = (t_snapShot*)malloc(sizeof(t_snapShot));
		if (mySnapShot == NULL)
		{
			LogError(strerror(GetLastError()));
			return NULL;
		}

		//*Loop of all processes
		for (i = 0; i < cProcesses; i++)
		{
			mySnapShot->process = getMemoryInfo(aProcesses[i]);

			if (mySnapShot->process != NULL)
			{
				addToProcessList(mySnapShot->process);
				processCount++;
			}
		}

		mySnapShot->processCounter = processCount;
		mySnapShot->process = headP;

		return mySnapShot;  //Return mySnapShot to create a linked list of SnapShots
	}
	else if (prevSnapShot != NULL)
	{
		//*Loop of all processes
		for (i = 0; i < cProcesses; i++)
		{
			sumSnapShots(prevSnapShot, aProcesses[i]);  //Send to a function that accumulates data
		}
		//ProcessIDBubbleSort();  //For Debug
		//PrintProcessIDList(HeadP);  //For Debug

		//prevSnapShot->process = headP;

		return prevSnapShot;
	}
	return;
}


void sumSnapShots(t_snapShot* prevSnapShot, DWORD processID)
{
	headD = NULL;
	tailD = NULL;
	int counterDLL = 0;
	t_Process* prevProcess = prevSnapShot->process;  //The processes of prev SnapShot
	t_Process* newProcess;  //Defining a pointer variable of type struct Process in order to enter the data into it
	size_t numConverted;

	HANDLE hProcess;
	PROCESS_MEMORY_COUNTERS pmc;

	//Open process in order to receive information
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

	//Get Process Name
	if (GetModuleFileNameEx(hProcess, 0, FoundProcessName, MAX_PATH))
	{
		// At this point, buffer contains the full path to the executable
		char processName[MAX_PATH];
		wcstombs_s(&numConverted, processName, MAX_PATH, FoundProcessName, MAX_PATH);
		if (strlen(processName) > 1)   //If the process name is less than 1 character, return NULL
		{
			newProcess = (t_Process*)malloc(sizeof(t_Process));
			if (!newProcess)
			{
				LogError(strerror(GetLastError()));
				return;
			}

			strcpy(newProcess->ProcessName, processName);
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		LogError(strerror(GetLastError()));
		return;
	}

	newProcess->ProcessID = processID;

	if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
	{
		newProcess->pmc.PageFaultCount = pmc.PageFaultCount;
		newProcess->pmc.WorkingSetSize = pmc.WorkingSetSize;
		newProcess->pmc.QuotaPagedPoolUsage = pmc.QuotaPagedPoolUsage;
		newProcess->pmc.QuotaPeakPagedPoolUsage = pmc.QuotaPeakPagedPoolUsage;
		newProcess->pmc.PagefileUsage = pmc.PagefileUsage;
	}

	// Get Dlls List
	

	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			if (GetModuleFileNameEx(hProcess, hMods[i], wDllName, MAX_PATH))
			{
				char getDllName[MAX_PATH];
				wcstombs_s(&numConverted, getDllName, MAX_PATH, wDllName, MAX_PATH);
				if (strlen(getDllName) > 1)   //If the DLL name is less than 1 character, return NULL
				{
					t_DLL* dllName = (t_DLL*)malloc(sizeof(t_DLL));
					if (dllName == NULL)
					{
						LogError(strerror(GetLastError()));
						return;
					}

					strcpy(dllName->DLLName, getDllName);
					addToDllList(dllName);
					counterDLL++;
				}
			}
		}
	}

	newProcess->numOfDLL = counterDLL;
	newProcess->DLL = headD;

	//Temporary variables with which we will run on the DLLs lists
	t_DLL* tempPrevDll = prevProcess->DLL;
	t_DLL* tempNewDll = newProcess->DLL;

	while (prevProcess != NULL)
	{
		//If its the same process, accumulate data
		if (prevProcess->ProcessID == newProcess->ProcessID && strcmp(prevProcess->ProcessName, newProcess->ProcessName) == 0)
		{
			prevProcess->pmc.PageFaultCount += newProcess->pmc.PageFaultCount;
			prevProcess->pmc.WorkingSetSize += newProcess->pmc.WorkingSetSize;
			prevProcess->pmc.QuotaPagedPoolUsage += newProcess->pmc.QuotaPagedPoolUsage;
			prevProcess->pmc.QuotaPeakPagedPoolUsage += newProcess->pmc.QuotaPeakPagedPoolUsage;
			prevProcess->pmc.PagefileUsage += newProcess->pmc.PagefileUsage;

			//Check the DLLs lists and add to prevProcess if its new
			while (tempNewDll != NULL)
			{
				tempPrevDll = prevProcess->DLL;  //Keeps the start (head) of the list
				while (tempPrevDll != NULL)
				{
					if (strcmp(tempPrevDll->DLLName, tempNewDll->DLLName) == 0)
					{
						break;
					}

					if (tempPrevDll->next == NULL)
					{
						t_DLL* newDll = (t_DLL*)malloc(sizeof(t_DLL));
						if (newDll == NULL)
						{
							LogError(strerror(GetLastError()));
							return;
						}

						*newDll = *tempNewDll;
						addNewDll(tempPrevDll, newDll);
						LogEvent("New DLL has been s added to the process");
						prevProcess->numOfDLL++;
					}
					tempPrevDll = tempPrevDll->next;
				}
				tempNewDll = tempNewDll->next;
			}

			//Free new Process and it's DLL List 
			resetProcess(newProcess);
			break;
		}

		prevProcess = prevProcess->next;
	}

	if (!prevProcess)  // new process
	{
		addNewProcess(tailP, newProcess);
		LogEvent("A new Process is added to the SnapShot");
		prevSnapShot->processCounter++;
	}

	CloseHandle(hProcess);
}
*/

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
	int i = 1;
	printf("Dll list:\n");
	while (temp)
	{
		printf("%d. %s\n",i, temp->DLLName);
		i++;
		temp = temp->next;
	}
}


void printProcessList(t_Process* proc) // a function that i was checking if process has been added
{
	t_Process* temp = proc;
	int i = 0;
	while (temp)
	{
		i++;

		printf("%d Process Id: %d Process: ", i, temp->ProcessID);
		printf("%s\n\n", temp->ProcessName);
		if (temp->DLL)
		{
			printDllList(temp->DLL);
		}
		temp = temp->next;
	}

}

