/*
 * common_func.cpp
 *
 *  Created on: 2013Äê9ÔÂ9ÈÕ
 *      Author: 1854
 */

#include "common_func.h"
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

FRAME_ROBOT_NAMESPACE_BEGIN

int32_t GetTimeString(char arrTimeString[])
{
	struct timeval tv;
 	gettimeofday(&tv, NULL);
	struct tm now = *localtime(&tv.tv_sec);

	int32_t nRetLen = sprintf(arrTimeString, "%04d/%02d/%02d %02d:%02d:%02d.%03ld",
			now.tm_year + 1900, now.tm_mon + 1, now.tm_mday, now.tm_hour,
			now.tm_min, now.tm_sec, tv.tv_usec / 1000);

	arrTimeString[nRetLen] = '\0';

	return nRetLen;
}

FRAME_ROBOT_NAMESPACE_END
