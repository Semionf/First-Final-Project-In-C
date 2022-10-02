#include "Save_Load.h"

void saveInFile(t_headerOfFile headerOfFile, t_snapShot* head)
{
	char fileName[100];
	t_snapShot* curr = head;
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
		fwrite(curr, sizeof(t_snapShot), 1, f);
		curr = curr->next;
	}
	fclose(f);
}

t_snapShot* loadFromFile(t_headerOfFile headerOfFile)
{
	char fileName[100];
	printf("Enter file name: ");
	scanf("%s", fileName);
	FILE* f = fopen(fileName, "rb");
	t_snapShot* snapShotList = NULL;
	t_snapShot* curr;
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
		snapShotList = addToList(curr);
	}
	fclose(f);
	printf("Linked list was uploaded.\n");
	return snapShotList;
}
