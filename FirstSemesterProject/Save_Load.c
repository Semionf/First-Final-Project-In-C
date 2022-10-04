#include "Save_Load.h"


t_DLL* headDLL = NULL;
t_DLL* tailDLL = NULL;

t_Process* headProcess = NULL;
t_Process* tailProcess = NULL;

void saveInFile(t_headerOfFile headerOfFile, t_snapShot* head)
{
	char fileName[100];
	t_snapShot* curr = head;
	t_Process* currProcess = head->process;
	t_DLL* currDLL = head->process->DLL;
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
	fwrite(&headerOfFile, sizeof(t_headerOfFile), 1, f);
	for (int i = 0; i < headerOfFile.ItemsCount; i++)
	{
		currProcess = head->process;
		fwrite(head, sizeof(t_snapShot), 1, f);
		while (head->process)
		{
			currDLL = head->process->DLL;
			fwrite(head->process, sizeof(t_Process), 1, f);
			while (head->process->DLL)
			{
				fwrite(head->process->DLL, sizeof(t_DLL), 1, f);
				head->process->DLL = head->process->DLL->next;
			}
			head->process->DLL = currDLL;
			head->process = head->process->next;
		}
		head->process = currProcess;
		head = head->next;
	}
	head = curr;
	fclose(f);
}

t_snapShot* loadFromFile(t_headerOfFile headerOfFile)
{

	headDLL = NULL;
	tailDLL = NULL;

	headProcess = NULL;
	tailProcess = NULL;
	char fileName[100];
	printf("Enter file name: ");
	scanf("%s", fileName);
	FILE* f = fopen(fileName, "rb");
	t_snapShot* snapShotList = NULL;
	t_snapShot* curr;
	t_Process* currProcess;
	t_DLL* currDLL;
	if (!f)
	{
		printf("File Error");
		exit(1);
	}
	fread(&headerOfFile, sizeof(t_headerOfFile), 1, f);
	if (!headerOfFile.ItemsCount)
	{
		Log("File is empty.\n");
		return;
	}
	for (int i = 0; i < headerOfFile.ItemsCount; i++)
	{
		curr = (t_snapShot*)malloc(sizeof(t_snapShot));
		if (!curr)
		{
			printf("Allocation Error");
			exit(1);
		}
		fread(curr, sizeof(t_snapShot), 1, f);
		currProcess = (t_Process*)malloc(sizeof(t_Process));
		if (!currProcess)
		{
			printf("Allocation Error");
			exit(1);
		}
		while (fread(currProcess, sizeof(t_Process), 1, f))
		{
			currDLL = (t_DLL*)malloc(sizeof(t_DLL));
			if (!currDLL)
			{
				printf("Allocation Error");
				exit(1);
			}
			while (fread(currDLL, sizeof(t_DLL), 1, f))
			{
				addToDllListFromFile(currDLL);
				currDLL = (t_DLL*)malloc(sizeof(t_DLL));
				if (!currDLL)
				{
					printf("Allocation Error");
					exit(1);
				}
			}
			free(currDLL);
			currProcess->DLL = headDLL;
			addToProcessListFromFile(currProcess);
			currProcess = (t_Process*)malloc(sizeof(t_Process));
			if (!currProcess)
			{
				printf("Allocation Error");
				exit(1);
			}
		}
		free(currProcess);
		curr = headProcess;
		snapShotList = addToList(curr);
	}
	fclose(f);
	printf("Linked list was uploaded.\n");
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
		tailProcess->next = process;
		process->prev = tailProcess;
		process->next = NULL;
		tailProcess = process;
	}
}