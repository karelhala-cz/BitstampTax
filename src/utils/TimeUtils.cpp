//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021-2022 Karel Hala
// License:	  MIT
//*********************************************************************************************************************

#include "TimeUtils.h"

unsigned GetYearFromTimeT(std::time_t const time)
{
	tm utcTime;
	localtime_s(&utcTime, &time);

	return 1900 + utcTime.tm_year;
}