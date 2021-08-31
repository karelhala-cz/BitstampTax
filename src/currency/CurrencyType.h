//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
// License:	  
//*********************************************************************************************************************

#pragma once
#include <stdint.h>

enum class E_CurrencyType : int8_t
{
	EUR,
	BTC,

	Count
};


char const * CurrencyTypeToStr(E_CurrencyType const type);
