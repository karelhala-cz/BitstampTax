//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021-2022 Karel Hala
// License:	  MIT
//*********************************************************************************************************************

#pragma once
#include "TradeType.h"
#include "Currency/CurrencyValue.h"
#include <ctime>

class C_TradeItem
{
public:
	C_TradeItem(size_t const inputFileLine, std::time_t	const time, T_CurrencyValueConstPtr && amount);

	virtual ~C_TradeItem() {}

	size_t					GetInputFileLine() const { return m_InputFileLine; }
	std::time_t				GetTime() const { return m_Time; }
	C_CurrencyValue	const & GetAmount() const { return *m_Amount; } 

	virtual void PrintData(std::ostringstream & str);

private:
	size_t m_InputFileLine;
	std::time_t	m_Time;
	T_CurrencyValueConstPtr	m_Amount;
};

typedef std::unique_ptr<C_TradeItem> T_TradeItemUniquePtr;