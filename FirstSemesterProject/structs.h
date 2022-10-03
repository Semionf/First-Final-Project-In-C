#include <Windows.h> 
#include <psapi.h>
#pragma once
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
	int processCounter;
	char timeOfSample[50];
	int sampleNumber;
	struct snapShot* next;
	struct snapShot* prev;
}t_snapShot;

typedef struct headerOfFile {
	int version;
	int serialNum;
	int ItemsCount;
	char reserve[100];
}t_headerOfFile;
