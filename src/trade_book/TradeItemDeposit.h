//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021-2022 Karel Hala
// License:	  MIT
//*********************************************************************************************************************

#include "TradeItem.h"


class C_TradeItemDeposit final: public C_TradeItem
{
	typedef C_TradeItem T_Super;

	virtual void PrintData(std::ostringstream & str) override;

public:
	C_TradeItemDeposit(size_t const inputFileLine, std::time_t const time, T_CurrencyValueConstPtr && amount);
};
