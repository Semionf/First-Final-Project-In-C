#include "menu.h"


t_snapShot* head = NULL;
t_snapShot* tail = NULL;

void menu()
{
	int startSec, endSec, diffSec = 0, diffMin = 0, startMin, endMin;
	time_t t;
	struct tm* timeInfo;
	int option = 0;
	int numberOfSnapShot = 0;
	t_snapShot* snapShot;
	
	do {
		printf("Please select one of the following options:\n1. One snap shot.\n2. Twenty snap shots.\n3. Generate html report.\n4.Reset collection.\n5. Save in file.\n6. Load from file.\n7. Quit.\n");
		scanf("%d", &option);
		switch (option)
		{
		case 1:
			numberOfSnapShot++;
			time(&t);
			timeInfo = localtime(&t);
			startSec = timeInfo->tm_sec;
			startMin = timeInfo->tm_min;
			snapShot = oneSnapShot(NULL,numberOfSnapShot);
			time(&t);
			
			timeInfo = localtime(&t);
			endSec = timeInfo->tm_sec;
			endMin = timeInfo->tm_min;
			diffMin = endMin - startMin;
			diffSec = endSec - startSec;
			if (diffSec < 0)
			{
				diffSec += 60;
				diffMin--;
			}
			snapShot->sampleNumber = numberOfSnapShot;
			sprintf(snapShot->timeOfSample, "Sample time is %d min and %d sec", diffMin, diffSec);
			addToList(snapShot);
			break;
		case 2:
			numberOfSnapShot++;
			time(&t);
			timeInfo = localtime(&t);
			startSec = timeInfo->tm_sec;
			startMin = timeInfo->tm_min;
			snapShot = twentySnapShots();
			time(&t);
			struct tm* timeInfo;
			timeInfo = localtime(&t);
			endSec = timeInfo->tm_sec;
			endMin = timeInfo->tm_min;
			diffMin = endMin - startMin;
			diffSec = endSec - startSec;
			snapShot->sampleNumber = numberOfSnapShot;
			sprintf(snapShot->timeOfSample, "Sample time is %d min and %d sec", diffMin, diffSec);
			addToList(snapShot);
			break;
		case 3:
			generateHtmlReport();
			break;
		case 4:
			resetCollection();
			break;
		case 5: 
			//saveInFile();
			break;
		case 6:
			//loadFromFile();
			break;
		case 7:
			resetCollection();
			break;
		default:
			printf("Wrong option chosen, please try again.");
			break;
		}
	} while (option != 7);
}

void addToList(t_snapShot* snapShot)
{
	if (!head)
	{
		snapShot->next = snapShot->prev = NULL;
		head = tail = snapShot;
	}
	else {
		tail->next = snapShot;
		snapShot->prev = tail;
		snapShot->next = NULL;
		tail = snapShot;
	}
}