//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021-2022 Karel Hala
// License:	  MIT
//*********************************************************************************************************************

#include "TradeItem.h"
#include <sstream>

C_TradeItem::C_TradeItem(size_t const inputFileLine, std::time_t const time, T_CurrencyValueConstPtr && amount)
	: m_InputFileLine(inputFileLine)
	, m_Time(std::move(time))
	, m_Amount(std::move(amount))
{
}

void C_TradeItem::PrintData(std::ostringstream & str)
{
	str << "time: ";
	str << m_Time;
	str << ", ";

	str << "amount: "; 
	m_Amount->PrintData(str);
}

