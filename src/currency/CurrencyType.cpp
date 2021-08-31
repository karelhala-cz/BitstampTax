//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
// License:	  
//*********************************************************************************************************************

#include "CurrencyType.h"

char const * CurrencyTypeToStr(E_CurrencyType const type)
{
	switch (type)
	{
		case E_CurrencyType::EUR:
		{
			return "EUR";
		}
		break;

		case E_CurrencyType::BTC:
		{
			return "BTC";
		}
		break;

		case E_CurrencyType::Count:
		default:
			break;
	}

	return "Invalid";
}
