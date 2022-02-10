//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021-2022 Karel Hala
// License:	  MIT
//*********************************************************************************************************************

#include "TradePair.h"

C_TradePair::C_TradePair(
	C_CurrencyValue const & amount,
	size_t const buyInputFileLine,
	C_CurrencyValue const & buyPrice,
	time_t const buyTime,
	size_t const sellInputFileLine,
	C_CurrencyValue const & sellPrice,
	time_t const sellTime)
	: m_Amount(amount)
	, m_BuyInputFileLine(buyInputFileLine)
	, m_BuyPrice(buyPrice)
	, m_BuyTime(buyTime)
	, m_SellInputFileLine(sellInputFileLine)
	, m_SellPrice(sellPrice)
	, m_SellTime(sellTime)
{
}