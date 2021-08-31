//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
// License:	  
//*********************************************************************************************************************

#include "TradeItem.h"


class C_TradeItemDeposit final: public C_TradeItem
{
	typedef C_TradeItem T_Super;

	virtual void PrintData(std::ostringstream & str) override;

public:
	C_TradeItemDeposit(std::time_t const time, T_CurrencyValueConstPtr && amount);
};
