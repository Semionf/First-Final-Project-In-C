#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <Windows.h> 
#include <psapi.h>
#include "Log.h"

typedef struct DLL {
	char DLLName[MAX_PATH];
	struct DLL* next;
	struct DLL* prev;
}t_DLL;

typedef struct Process {
	PROCESS_MEMORY_COUNTERS pmc;
	int ProcessID;
	char ProcessName[MAX_PATH];
	t_DLL* DLL;
	int numOfDLL;
	struct Process* next;
	struct Process* prev;
}t_Process;

typedef struct snapShot {
	t_Process* process;
	char timeOfSample[50];
	int sampleNumber;
	struct snapShot* next;
	struct snapShot* prev;
}t_snapShot;

t_Process* getMemoryInfo(DWORD );
t_snapShot* GetProcessesInfo(t_snapShot*);
t_snapShot* sumProcessesAndDLL(t_snapShot* , t_snapShot* );
void addToDllList(t_DLL*);
void addToProcessList(t_Process*);
void addNewDll(t_Process* , t_Process* );
void addNewProcess(t_Process* , t_Process* );