#include "menu.h"
Dictionary* dict = NULL;
//user menu
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
		case 1: //One snapShot
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
		case 2: //Twenty snapshots summed into one
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
			printProcessList(snapShotList->process);
			break;
		case 3:	//Long summing snapshots till user type "End"
			numberOfSnapShots++;
			headerOfFile->ItemsCount = numberOfSnapShots;
			time(&t);
			timeInfo = localtime(&t);
			sprintf(str, "Date and time of sample: %d %d %d - %02d:%02d", timeInfo->tm_year + 1900, timeInfo->tm_mon + 1, timeInfo->tm_mday, timeInfo->tm_hour, timeInfo->tm_min);
			snapShot = LongSnapShot();
			strcpy(snapShot->timeOfSample, str);
			snapShot->sampleNumber = numberOfSnapShots;
			snapShotList = addToList(snapShot);
		case 4: // Making a dictionary when Dll names are keys and processes are values, then generating html
			dict = buildDictionary(snapShotList);
			generateHtmlReport(snapShotList, dict);
			break;
		case 5: // reset all snapshots generated
			resetCollection(snapShotList, headerOfFile, dict);
			headerOfFile->ItemsCount = 0;
			snapShotList = NULL;
			addToList(NULL);
			break;
		case 6: // save in file
			saveInFile(headerOfFile, snapShotList);
			break;
		case 7: // load from file
			snapShot = loadFromFile(headerOfFile);
			addToList(snapShot);
			break;
		case 8: // Delete all and Quit
			resetCollection(snapShotList, headerOfFile, dict);
			numberOfSnapShots = 0;
			headerOfFile->ItemsCount = numberOfSnapShots;
			break;
		default:
			printf("Wrong option chosen, please try again.");
			break;
		}
	} while (option != 8);
}

