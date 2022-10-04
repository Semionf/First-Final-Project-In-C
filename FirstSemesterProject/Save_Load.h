#include "structs.h"
#include "Log.h"
#include "LinkedLists.h"

#pragma once

void saveInFile(t_headerOfFile ,t_snapShot* );
t_snapShot* loadFromFile(t_headerOfFile);
void addToProcessListFromFile(t_Process*);
void addToDllListFromFile(t_DLL*);