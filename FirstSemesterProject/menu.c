#include "menu.h"
Dictionary* dict = NULL;
//user menu
void menu()
{
	time_t t;
	char str[50];
	struct tm* timeInfo;
	char option;
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
		printf("Please select one of the following options:\nA. One snap shot.\nT. Twenty snap shots.\nL. Long snap shot \nH. Generate html report\nC. Reset collection.\nS. Save in file.\nU. Load from file.\nQ. Quit.\n");
		(void)scanf(" %c", &option);
		switch(option)
		{
		case 'A': //One snapShot
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
		case 'T': //Twenty snapshots summed into one
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
		case 'L':	//Long summing snapshots till user type "End"
			numberOfSnapShots++;
			headerOfFile->ItemsCount = numberOfSnapShots;
			time(&t);
			timeInfo = localtime(&t);
			sprintf(str, "Date and time of sample: %d %d %d - %02d:%02d", timeInfo->tm_year + 1900, timeInfo->tm_mon + 1, timeInfo->tm_mday, timeInfo->tm_hour, timeInfo->tm_min);
			snapShot = LongSnapShot();
			strcpy(snapShot->timeOfSample, str);
			snapShot->sampleNumber = numberOfSnapShots;
			snapShotList = addToList(snapShot);
		case 'H': // Making a dictionary when Dll names are keys and processes are values, then generating html
			dict = buildDictionary(snapShotList);
			generateHtmlReport(snapShotList, dict);
			break;
		case 'C': // reset all snapshots generated
			resetCollection(snapShotList, headerOfFile, dict);
			headerOfFile->ItemsCount = 0;
			snapShotList = NULL;
			addToList(NULL);
			break;
		case 'S': // save in file
			saveInFile(headerOfFile, snapShotList);
			break;
		case 'U': // load from file
			snapShot = loadFromFile(headerOfFile);
			addToList(snapShot);
			break;
		case 'Q': // Delete all and Quit
			resetCollection(snapShotList, headerOfFile, dict);
			numberOfSnapShots = 0;
			headerOfFile->ItemsCount = numberOfSnapShots;
			break;
		default:
			printf("Wrong option chosen, please try again.");
			break;
		}
	} while (option != 'Q');
}

