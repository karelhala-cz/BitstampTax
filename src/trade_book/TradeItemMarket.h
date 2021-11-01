//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
// License:	  
//*********************************************************************************************************************

#pragma once
#include "TradeItem.h"
#include "TradeType.h"
#include "Currency/CurrencyValue.h"

class C_TradeItemMarket : public C_TradeItem
{
	typedef C_TradeItem T_Super;

public:
	C_TradeItemMarket(size_t const inputFileLine,
		std::time_t const time,
		T_CurrencyValueConstPtr && amount,
		T_CurrencyValueConstPtr && value,
		T_CurrencyValueConstPtr && rate,
		T_CurrencyValueConstPtr && fee,
		E_TradeType const type);

	C_CurrencyValue	const & GetValue() const { return *m_Value; } 
	C_CurrencyValue	const & GetRate() const { return *m_Rate; }
	bool					IsFeeValid() const { return m_Fee != nullptr; }
	C_CurrencyValue	const & GetFee() const { return *m_Fee; }
	E_TradeType				GetTradeType() const { return m_Type; }

	virtual void PrintData(std::ostringstream & str) override;

private:
	T_CurrencyValueConstPtr	m_Value;
	T_CurrencyValueConstPtr	m_Rate;
	T_CurrencyValueConstPtr	m_Fee;
	E_TradeType				m_Type;
};
