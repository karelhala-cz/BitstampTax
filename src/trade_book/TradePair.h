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
		C_CurrencyValue const & buyPrice,
		time_t const buyTime,
		C_CurrencyValue const & sellPrice,
		time_t const sellTime);

	C_CurrencyValue	const & GetAmount() const { return m_Amount; } 

	C_CurrencyValue	const & GetBuyPrice() const { return m_BuyPrice; }
	time_t					GetBuyTime() const { return m_BuyTime; }

	C_CurrencyValue	const & GetSellPrice() const { return m_SellPrice; }
	time_t					GetSellTime() const { return m_SellTime; }

private:
	C_CurrencyValue	m_Amount;
	
	C_CurrencyValue	m_BuyPrice;
	time_t			m_BuyTime;

	C_CurrencyValue	m_SellPrice;
	time_t			m_SellTime;
};
