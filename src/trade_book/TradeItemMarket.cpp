//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021-2022 Karel Hala
// License:	  MIT
//*********************************************************************************************************************

#include "TradeItemMarket.h"

C_TradeItemMarket::C_TradeItemMarket(size_t const inputFileLine,
	std::time_t const time,
	T_CurrencyValueConstPtr&& amount,
	T_CurrencyValueConstPtr&& value,
	T_CurrencyValueConstPtr&& rate,
	T_CurrencyValueConstPtr&& fee,
	E_TradeType const type)
	: T_Super(inputFileLine, time, std::move(amount))
	, m_Value(std::move(value))
	, m_Rate(std::move(rate))
	, m_Fee(std::move(fee))
	, m_Type(type)
{
}

//virtual
void C_TradeItemMarket::PrintData(std::ostringstream & str)
{
	str << "[market] ";
	
	T_Super::PrintData(str);

	str << ", value: ";
	m_Value->PrintData(str);

	str << ", rate: ";
	m_Rate->PrintData(str);

	str << ", fee: ";
	m_Fee->PrintData(str);

	str << ", type: ";
	str << TradeTypeToStr(m_Type);
}

