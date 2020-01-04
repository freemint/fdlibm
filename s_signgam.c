#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

int signgam = 0;

int *__signgam (void)
{
	return &signgam;
}
