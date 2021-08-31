//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
// License:	  
//*********************************************************************************************************************

#include "TradeType.h"

char const * TradeTypeToStr(E_TradeType const type)
{
	switch (type)
	{
		case E_TradeType::Buy:
		{
			return "buy";
		}
		break;

		case E_TradeType::Sell:
		{
			return "sell";
		}
		break;

		case E_TradeType::Count:
		default:
			break;
	}

	return "Invalid";
}
