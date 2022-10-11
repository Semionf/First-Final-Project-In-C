#include "snapShot.h"

t_snapShot* oneSnapShot(t_snapShot* oldSnapShot)
{
	t_snapShot* snapShot;
	snapShot = GetProcessesInfo(oldSnapShot);
	return snapShot;
}

t_snapShot* twentySnapShots()
{
	t_snapShot* snapShot = NULL;
	for (int i = 0; i < 20; i++)
	{
		snapShot = oneSnapShot(snapShot);
		printf("\n\nRotation number %d \n\n",i);
		Sleep(1000);
	}
	return snapShot;
}

t_snapShot* LongSnapShot()
{
	int stop = 0;
	char ch[10];
	t_snapShot* snapShot = NULL;
	while(stop != 1)
	{
		snapShot = oneSnapShot(snapShot);
		Sleep(1000);
		if (kbhit())
		{
			printf("Write End in order to finish Long snap shot\n");
			(void)scanf("%s",ch);
			if (strcmp(ch,"End")==0)
			{
				stop = 1;
			}
		}
	}
	return snapShot;
}
