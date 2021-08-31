//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
// License:	  
//*********************************************************************************************************************

#pragma once

enum class E_TradeType
{
	Buy,
	Sell,

	Count
};

char const * TradeTypeToStr(E_TradeType const type);
