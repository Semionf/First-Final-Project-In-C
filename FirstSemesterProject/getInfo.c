#include "getInfo.h"


t_DLL* headD = NULL;
t_DLL* tailD = NULL;

t_Process* headP = NULL;
t_Process* tailP = NULL;

void getMemoryInfo(DWORD processID)
{
	t_Process* Process;
	t_DLL* DLL;
	
	size_t numConverted;

	HANDLE hProcess;
	PROCESS_MEMORY_COUNTERS pmc;

	// Open process in order to receive information
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID);
	if (NULL == hProcess)
	{
		LogError(strerror(GetLastError()));
		return;
	}
	Process = (t_Process*)malloc(sizeof(t_Process));
	if (!Process)
	{
		LogError(strerror(GetLastError()));
		return;
	}
	HMODULE hMods[1024];
	DWORD cbNeeded;
	TCHAR FoundProcessName[MAX_PATH];
	TCHAR wDllName[MAX_PATH];
	
	int i = 0;
	// Get Process Name
	if (GetModuleFileNameEx(hProcess, 0, FoundProcessName, MAX_PATH))
	{
		// At this point, buffer contains the full path to the executable
		wcstombs_s(&numConverted, Process->ProcessName, MAX_PATH, FoundProcessName, MAX_PATH);
	}
	else
	{
		// You better call GetLastError() here
		// Write To log
		LogError(strerror(GetLastError()));
		return;
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
				DLL = (t_DLL*)malloc(sizeof(t_DLL));
				if (!DLL)
				{
					LogError(strerror(GetLastError()));
					return;
				}
				// * Get the module name and handle value.

				// Convert wChar to regular char array (string)
				
				wcstombs_s(&numConverted, DLL->DLLName, MAX_PATH, wDllName, MAX_PATH);
				addToDllList(DLL);
			}
		}
		Process->DLL = headD;
		Process->numOfDLL = i;
	}
	Process->ProcessID = processID;
	if (i == 0) // no dll for this process
	{
		Process->numOfDLL = 0;
		Process->DLL = NULL;
	}
	addToProcessList(Process);
	CloseHandle(hProcess);
}
t_snapShot* GetProcessesInfo(t_snapShot* oldSnapShot)
{
	// Get Processes

	t_snapShot* snapShot;
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;

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
		getMemoryInfo(aProcesses[i]);
	}

	snapShot->process = headP;
	
	// For each Process to get its Memory Information
	if (oldSnapShot)
	{
		snapShot = sumProcessesAndDLL(oldSnapShot, snapShot);
	}
	return snapShot;
}

t_snapShot* sumProcessesAndDLL(t_snapShot* old, t_snapShot* new)
{
	//t_snapShot* oldTemp;
	//t_snapShot* newTemp;
	return old;
}

void addToDllList(t_DLL* DLL)
{
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