#include "Log.h"	
#include "snapShot.h"
#include "Dictionary.h"
#pragma once
void generateHtmlReport(t_snapShot*);
void homePage(t_snapShot*, Dictionary*);
void generateSnapShotPage(t_snapShot*);
void generateDLLPage(Dictionary*);
long calculateAvgWorkingSetSize(t_snapShot*);