#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <Windows.h> 
#include <psapi.h>
#include "Log.h"
#include "structs.h"
#include "resetCollection.h"
#pragma once
void freeProcess(t_Process* );
t_snapShot* sumProcessesAndDLL(t_snapShot*, t_snapShot*);
t_Process* getMemoryInfo(DWORD );
t_snapShot* GetProcessesInfo(t_snapShot*);
void printDllList(t_DLL* );
void printProcessList(t_Process* );
void addToDllList(t_DLL*);
void addToProcessList(t_Process*);
void addNewDll(t_DLL* , t_DLL* );
void addNewProcess(t_Process* , t_Process* );

extern t_Process* headP;
extern t_Process* tailP;