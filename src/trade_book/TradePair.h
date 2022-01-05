//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
// License:	  
//*********************************************************************************************************************

#pragma once
#include "Currency/CurrencyValue.h"


class C_TradePair
{
public:
	C_TradePair(
		C_CurrencyValue const & amount,
		size_t const buyInputFileLine,
		C_CurrencyValue const & buyPrice,
		time_t const buyTime,
		size_t const sellInputFileLine,
		C_CurrencyValue const & sellPrice,
		time_t const sellTime);

	C_CurrencyValue	const & GetAmount() const { return m_Amount; } 

	size_t					GetBuyInputFileLine() const { return m_BuyInputFileLine; }
	C_CurrencyValue	const & GetBuyPrice() const { return m_BuyPrice; }
	time_t					GetBuyTime() const { return m_BuyTime; }

	size_t					GetSellInputFileLine() const { return m_SellInputFileLine; }
	C_CurrencyValue	const & GetSellPrice() const { return m_SellPrice; }
	time_t					GetSellTime() const { return m_SellTime; }

private:
	C_CurrencyValue	m_Amount;
	
	size_t			m_BuyInputFileLine;
	C_CurrencyValue	m_BuyPrice;
	time_t			m_BuyTime;

	size_t			m_SellInputFileLine;
	C_CurrencyValue	m_SellPrice;
	time_t			m_SellTime;
};
