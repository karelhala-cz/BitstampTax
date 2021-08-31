//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
// License:	  
//*********************************************************************************************************************

#include "TradeItemDeposit.h"

C_TradeItemDeposit::C_TradeItemDeposit(std::time_t const time, T_CurrencyValueConstPtr && amount)
	: T_Super(time, std::move(amount))
{
}

//virtual
void C_TradeItemDeposit::PrintData(std::ostringstream & str)
{
	str << "[deposit] ";
	
	T_Super::PrintData(str);
}

