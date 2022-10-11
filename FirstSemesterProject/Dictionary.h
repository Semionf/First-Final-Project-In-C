#include "structs.h"
#include "Log.h"
Dictionary* buildDictionary(t_snapShot*);
void addToDictionary(char*, t_Process*);
void addNewValue(Dictionary*, t_Process*);
int countNumOfDLL(Dictionary*);
P_Dictionary* buildProcessDictionary(t_snapShot*);
void addProcessToDictionary(t_Process*);
int countNumOfProcesses(P_Dictionary*);
