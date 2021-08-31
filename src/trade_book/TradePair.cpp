//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
// License:	  
//*********************************************************************************************************************

#include "TradePair.h"

C_TradePair::C_TradePair(
	C_CurrencyValue const & amount,
	C_CurrencyValue const & buyPrice,
	time_t const buyTime,
	C_CurrencyValue const & sellPrice,
	time_t const sellTime)
	: m_Amount(amount)
	, m_BuyPrice(buyPrice)
	, m_BuyTime(buyTime)
	, m_SellPrice(sellPrice)
	, m_SellTime(sellTime)
{
}