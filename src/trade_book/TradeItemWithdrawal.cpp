//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
// License:	  
//*********************************************************************************************************************

#include "TradeItemWithdrawal.h"

C_TradeItemWithdrawal::C_TradeItemWithdrawal(size_t const inputFileLine,
	std::time_t const time,
	T_CurrencyValueConstPtr && amount,
	T_CurrencyValueConstPtr && fee)
	: T_Super(inputFileLine, time, std::move(amount))
	, m_Fee(std::move(fee))
{
}

//virtual
void C_TradeItemWithdrawal::PrintData(std::ostringstream & str)
{
	str << "[withdrawal] ";
	
	T_Super::PrintData(str);

	str << ", fee: ";
	m_Fee->PrintData(str);
}

