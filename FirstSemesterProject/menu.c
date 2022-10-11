#include "menu.h"
Dictionary* dict = NULL;

void menu()
{
	time_t t;
	char str[50];
	struct tm* timeInfo;
	int option = 0;
	int numberOfSnapShots = 0;
	t_snapShot* snapShot;
	t_snapShot* snapShotList = NULL;
	t_headerOfFile* headerOfFile = (t_headerOfFile*)malloc(sizeof(t_headerOfFile));
	if (!headerOfFile)
	{
		LogError(strerror(GetLastError()));
		return 1;
	}
	headerOfFile->version = 1;
	headerOfFile->ItemsCount = 0;
	do {
		printf("Please select one of the following options:\n1. One snap shot.\n2. Twenty snap shots.\n3. Long snap shot \n4. Generate html report\n5. Reset collection.\n6. Save in file.\n7. Load from file.\n8. Quit.\n");
		scanf("%d", &option);
		switch (option)
		{
		case 1:
			numberOfSnapShots = headerOfFile->ItemsCount;
			numberOfSnapShots++;
			headerOfFile->ItemsCount = numberOfSnapShots;
			time(&t);
			timeInfo = localtime(&t);
			sprintf(str, "Date and time of sample: %d %d %d - %02d:%02d", timeInfo->tm_year + 1900, timeInfo->tm_mon + 1, timeInfo->tm_mday, timeInfo->tm_hour, timeInfo->tm_min);
			snapShot = oneSnapShot(NULL);
			strcpy(snapShot->timeOfSample, str);
			snapShot->sampleNumber = numberOfSnapShots;
			shakeSort(snapShot);
			snapShotList = addToList(snapShot);
			break;
		case 2:
			numberOfSnapShots = headerOfFile->ItemsCount;
			numberOfSnapShots++;
			headerOfFile->ItemsCount = numberOfSnapShots;
			time(&t);
			timeInfo = localtime(&t);
			sprintf(str, "Date and time of sample: %d %d %d - %02d:%02d", timeInfo->tm_year + 1900, timeInfo->tm_mon + 1, timeInfo->tm_mday, timeInfo->tm_hour, timeInfo->tm_min);
			snapShot = twentySnapShots();
			strcpy(snapShot->timeOfSample, str);
			snapShot->sampleNumber = numberOfSnapShots;
			shakeSort(snapShot);
			snapShotList = addToList(snapShot);
			break;
		case 3:
			numberOfSnapShots++;
			headerOfFile->ItemsCount = numberOfSnapShots;
			time(&t);
			timeInfo = localtime(&t);
			sprintf(str, "Date and time of sample: %d %d %d - %02d:%02d", timeInfo->tm_year + 1900, timeInfo->tm_mon + 1, timeInfo->tm_mday, timeInfo->tm_hour, timeInfo->tm_min);
			snapShot = LongSnapShot();
			strcpy(snapShot->timeOfSample, str);
			snapShot->sampleNumber = numberOfSnapShots;
			snapShotList = addToList(snapShot);
		case 4:
			dict = buildDictionary(snapShotList);
			generateHtmlReport(snapShotList, dict);
			break;
		case 5:
			resetCollection(snapShotList, headerOfFile);
			headerOfFile->ItemsCount = 0;
			snapShotList = NULL;
			addToList(NULL);
			break;
		case 6: 
			saveInFile(headerOfFile, snapShotList);
			break;
		case 7:
			snapShotList = loadFromFile(headerOfFile);
			break;
		case 8:
			resetCollection(snapShotList, headerOfFile);
			numberOfSnapShots = 0;
			headerOfFile->ItemsCount = numberOfSnapShots;
			break;
		default:
			printf("Wrong option chosen, please try again.");
			break;
		}
	} while (option != 9);
}

