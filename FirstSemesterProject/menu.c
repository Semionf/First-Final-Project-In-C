#include "menu.h"




void menu()
{
	time_t t;
	char str[50];
	struct tm* timeInfo;
	int option = 0;
	int numberOfSnapShot = 0;
	t_snapShot* snapShot;
	t_snapShot* snapShotList = NULL;
	t_headerOfFile headerOfFile;
	headerOfFile.version = 1;
	do {
		printf("Please select one of the following options:\n1. One snap shot.\n2. Twenty snap shots.\n3. Generate html report.\n4.Reset collection.\n5. Save in file.\n6. Load from file.\n7. Quit.\n");
		scanf("%d", &option);
		switch (option)
		{
		case 1:
			numberOfSnapShot++;
			headerOfFile.ItemsCount = numberOfSnapShot;
			timeInfo = localtime(&t);
			time(&t);
			sprintf(str, "Date and time of sample: %d %d %d - %02d:%02d:%02d", timeInfo->tm_year + 1900, timeInfo->tm_mon + 1, timeInfo->tm_mday, timeInfo->tm_hour, timeInfo->tm_min);
			snapShot = oneSnapShot(NULL,numberOfSnapShot);
			strcpy(snapShot->timeOfSample, str);
			snapShot->sampleNumber = numberOfSnapShot;
			snapShotList = addToList(snapShot);
			break;
		case 2:
			numberOfSnapShot++;
			headerOfFile.ItemsCount = numberOfSnapShot;
			timeInfo = localtime(&t);
			time(&t);
			sprintf(str, "Date and time of sample: %d %d %d - %02d:%02d:%02d", timeInfo->tm_year + 1900, timeInfo->tm_mon + 1, timeInfo->tm_mday, timeInfo->tm_hour, timeInfo->tm_min);
			snapShot = twentySnapShots();
			strcpy(snapShot->timeOfSample, str);
			snapShot->sampleNumber = numberOfSnapShot;
			snapShotList = addToList(snapShot);
			break;
		case 3:
			generateHtmlReport(snapShotList);
			break;
		case 4:
			resetCollection(snapShotList);
			break;
		case 5: 
			saveInFile(headerOfFile, snapShotList);
			resetCollection(snapShotList);
			break;
		case 6:
			snapShotList = loadFromFile(headerOfFile);
			break;
		case 7:
			resetCollection(snapShotList);
			headerOfFile.ItemsCount = 0;
			break;
		default:
			printf("Wrong option chosen, please try again.");
			break;
		}
	} while (option != 7);
}

