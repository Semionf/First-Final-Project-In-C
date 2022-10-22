#include "Save_Load.h"


t_DLL* headDLL = NULL;
t_DLL* tailDLL = NULL;

t_Process* headProcess = NULL;
t_Process* tailProcess = NULL;
// save and load functions for the snapshots
void saveInFile(t_headerOfFile* headerOfFile, t_snapShot* head)
{
	char fileName[100];
	t_snapShot* curr = head;
	t_Process* currProcess = head->process;
	t_DLL* currDLL = currProcess->DLL;
	printf("Enter file name: ");
	scanf("%s", fileName);
	if (!curr)
	{
		Log("The List is empty \n");
		return;
	}
	FILE* f = fopen(fileName, "wb");
	if (!f)
	{
		LogError(strerror(GetLastError()));
		return;
	}
	fwrite(headerOfFile, sizeof(t_headerOfFile), 1, f);
	for (int i = 0; i < headerOfFile->ItemsCount; i++)
	{
		fwrite(curr, sizeof(t_snapShot), 1, f);
		currProcess = curr->process;
		while (currProcess)
		{
			fwrite(currProcess, sizeof(t_Process), 1, f);
			currDLL = currProcess->DLL;
			while (currDLL)
			{
				fwrite(currDLL, sizeof(t_DLL), 1, f);
				currDLL = currDLL->next;
			}
			currProcess = currProcess->next;
		}
		curr = curr->next;
	}
	fclose(f);
}

t_snapShot* loadFromFile(t_headerOfFile* headerOfFile)
{

	headDLL = NULL;
	tailDLL = NULL;
	int itemCount = 0;
	headProcess = NULL;
	tailProcess = NULL;
	char fileName[100];
	printf("Enter file name: ");
	scanf("%s", fileName);
	FILE* f = fopen(fileName, "rb");
	t_snapShot* snapShotList = NULL;
	if (headerOfFile == NULL)
	{
		headerOfFile = (t_headerOfFile*)malloc(sizeof(t_headerOfFile));
	}
	else
	{
		itemCount = headerOfFile->ItemsCount;
	}
	t_snapShot* curr;
	t_Process* currProcess;
	t_DLL* currDLL = NULL;
	if (!f)
	{
		LogError(strerror(GetLastError()));
		exit(1);
	}
	fread(headerOfFile, sizeof(t_headerOfFile), 1, f);
	if (!headerOfFile)
	{
		Log("File is empty.\n");
		return;
	}
	for (int i = 0; i < headerOfFile->ItemsCount; i++)
	{
		curr = (t_snapShot*)malloc(sizeof(t_snapShot));
		if (!curr)
		{
			LogError(strerror(GetLastError()));
			exit(1);
		}
		fread(curr, sizeof(t_snapShot), 1, f);
		currProcess = (t_Process*)malloc(sizeof(t_Process));
		if (!currProcess)
		{
			LogError(strerror(GetLastError()));
			exit(1);
		}
		for(int j = 0; j < curr->processCounter; j++)
		{
			currProcess = (t_Process*)malloc(sizeof(t_Process));
			if (!currProcess)
			{
				printf("Allocation Error");
				exit(1);
			}
			fread(currProcess, sizeof(t_Process), 1, f);
			currDLL = (t_DLL*)malloc(sizeof(t_DLL));
			if (!currDLL)
			{
				LogError(strerror(GetLastError()));
				exit(1);
			}
			for (int k = 0; k < currProcess->numOfDLL; k++)
			{
				currDLL = (t_DLL*)malloc(sizeof(t_DLL));
				if (!currDLL)
				{
					LogError(strerror(GetLastError()));
					exit(1);
				}
				fread(currDLL, sizeof(t_DLL), 1, f);
				addToDllListFromFile(currDLL);
			}
			currProcess->DLL = headDLL;
			headDLL = NULL;
			tailDLL = NULL;
			addToProcessListFromFile(currProcess);
		}
		curr->process = headProcess;
		snapShotList = addToList(curr);
		headProcess = NULL;
		tailProcess = NULL;
	}
	fclose(f);
	printf("Linked list was uploaded.\n");
	headerOfFile->ItemsCount += itemCount;
	return snapShotList;
}



void addToDllListFromFile(t_DLL* DLL)
{
	if (!DLL)
	{
		return;
	}
	if (!headDLL)
	{
		DLL->next = DLL->prev = NULL;
		headDLL = tailDLL = DLL;
	}
	else
	{
		tailDLL->next = DLL;
		DLL->prev = tailDLL;
		DLL->next = NULL;
		tailDLL = DLL;
	}

}

void addToProcessListFromFile(t_Process* process)
{
	if (!headProcess)
	{
		process->next = process->prev = NULL;
		headProcess = tailProcess = process;
	}
	else
	{
		process->next = NULL;
		tailProcess->next = process;
		process->prev = tailProcess;
		tailProcess = process;
	}
}