#include "snapShot.h"

t_snapShot* oneSnapShot(t_snapShot* oldSnapShot, int num)
{
	t_snapShot* snapShot;
	printf("%d", num);
	snapShot = GetProcessesInfo(oldSnapShot);
	return snapShot;
}

t_snapShot* twentySnapShots()
{
	t_snapShot* snapShot = NULL;
	for (int i = 0; i < 20; i++)
	{
		snapShot = oneSnapShot(snapShot,i);
		Sleep(1000);
	}
	return snapShot;
}