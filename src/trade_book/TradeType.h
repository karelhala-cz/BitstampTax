//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021-2022 Karel Hala
// License:	  MIT
//*********************************************************************************************************************

#pragma once

enum class E_TradeType
{
	Buy,
	Sell,

	Count
};

char const * TradeTypeToStr(E_TradeType const type);
