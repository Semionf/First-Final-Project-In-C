#include "Log.h"	
#include "snapShot.h"
#include "Dictionary.h"
#pragma once
void generateHtmlReport(t_snapShot*);
void homePage(t_snapShot*, Dictionary*);
void generateSnapShotPage(t_snapShot*, char*);
void generateDLLPage(Dictionary*, char*);
long calculateAvgWorkingSetSize(t_snapShot*);
long calculateAvgWorkingSetSizeForCurrentSnapShot(t_snapShot* );
char* SamplesList(t_snapShot*);
char* DLLList(Dictionary*, int);
char* addOption(t_DLL*);