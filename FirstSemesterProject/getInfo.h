#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <Windows.h> 
#include <psapi.h>
#include "Log.h"
#include "structs.h"
#pragma once

t_Process* getMemoryInfo(DWORD );
t_snapShot* GetProcessesInfo(t_snapShot*);
t_snapShot* sumProcessesAndDLL(t_snapShot* , t_snapShot* );
void addToDllList(t_DLL*);
void addToProcessList(t_Process*);
void addNewDll(t_Process* , t_Process* );
void addNewProcess(t_Process* , t_Process* );