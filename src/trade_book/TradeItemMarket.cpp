//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
// License:	  
//*********************************************************************************************************************

#include "TradeItemMarket.h"

C_TradeItemMarket::C_TradeItemMarket(std::time_t const time,
	T_CurrencyValueConstPtr&& amount,
	T_CurrencyValueConstPtr&& value,
	T_CurrencyValueConstPtr&& rate,
	T_CurrencyValueConstPtr&& fee,
	E_TradeType const type)
	: T_Super(time, std::move(amount))
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
