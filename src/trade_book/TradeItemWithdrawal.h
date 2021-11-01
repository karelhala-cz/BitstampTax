//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
// License:	  
//*********************************************************************************************************************

#include "TradeItem.h"


class C_TradeItemWithdrawal final: public C_TradeItem
{
	typedef C_TradeItem T_Super;

	virtual void PrintData(std::ostringstream & str) override;

public:
	C_TradeItemWithdrawal(size_t const inputFileLine,
		std::time_t const time,
		T_CurrencyValueConstPtr && amount,
		T_CurrencyValueConstPtr && fee);

private:
	T_CurrencyValueConstPtr	m_Fee;
};
