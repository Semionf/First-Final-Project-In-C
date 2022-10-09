#include "Log.h"	
#include "snapShot.h"
#pragma once
void generateHtmlReport(t_snapShot*);
void homePage(t_snapShot*, Dictionary*);
void generateSnapShotPage(t_snapShot*);
void generateDLLPage(Dictionary*);
int countNumOfDLL(t_Process*);
long calculateAvgWorkingSetSize(t_snapShot*);