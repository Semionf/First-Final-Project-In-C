#include "Log.h"	
#include "Dictionary.h"
#include "resetCollection.h"
#pragma once
void generateHtmlReport(t_snapShot*,Dictionary*);
void homePage(t_snapShot*, Dictionary*);
void generateSnapShotPage(t_snapShot*, char*);
void generateDLLPage(Dictionary*, char*);
long long calculateAvgWorkingSetSize(t_snapShot*);
long long calculateAvgWorkingSetSizeForCurrentSnapShot(t_snapShot* );
char* SamplesList(t_snapShot*, Dictionary*);
char* DLLList(Dictionary*, int);
char* addOption(t_DLL*);
int highestMemoryIndex(t_snapShot*);
void generateAboutMePage();