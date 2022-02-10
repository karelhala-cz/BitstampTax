//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021-2022 Karel Hala
// License:	  MIT
//*********************************************************************************************************************

#include "TradeItemDeposit.h"

C_TradeItemDeposit::C_TradeItemDeposit(size_t const inputFileLine, std::time_t const time, T_CurrencyValueConstPtr && amount)
	: T_Super(inputFileLine, time, std::move(amount))
{
}

//virtual
void C_TradeItemDeposit::PrintData(std::ostringstream & str)
{
	str << "[deposit] ";
	
	T_Super::PrintData(str);
}

