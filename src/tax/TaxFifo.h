//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
// License:	  
//*********************************************************************************************************************

#pragma once

#include "Tax.h"
#include "trade_book/TradeBook.h"
#include "trade_book/TradePair.h"
#include "trade_book/TradeItemMarket.h"
#include <vector>
#include <string>
#include <assert.h>

class C_TaxFifo
{
	typedef std::vector<C_TradeItemMarket const *> T_TradeItems;

public: 
	C_TaxFifo(T_CurrencyType const & taxCurrency);

	bool Process(C_TradeBook const & book);

	size_t GetPairsCount() const { return m_Pairs.size(); }
	C_TradePair const & GetTradePair(size_t const index) const { assert(index < m_Pairs.size()); return m_Pairs.at(index); }

	size_t GetTaxCount() const { return m_Tax.size(); }

	template<typename Fn>
	void EnumerateTradePairs(Fn & fn) const;

	template<typename Fn>
	void EnumerateTaxes(Fn & fn) const;

	void PrintTax(std::ostringstream & str);

	std::string const & GetErrorMsg() const { return m_ErrorMsg; }

private:
	C_Tax::T_Year GetYear(std::time_t const time);
	C_Tax &	GetTaxYear(C_Tax::T_Year const year);

	bool FindPairs(T_TradeItems const & buys, T_TradeItems const & sells);
	void AddPair(C_CurrencyValue const & amount, C_TradeItemMarket const & buyItem, C_TradeItemMarket const & sellItem);

	size_t GetFirstItemIndexForType(T_CurrencyType const & type, std::time_t const maxTime, T_TradeItems const & items) const;
	size_t GetNextItemIndexForType(T_CurrencyType const & type, std::time_t const maxTime, size_t const index, T_TradeItems const & items) const;

	void ProcessPairs();

private:
	T_CurrencyType m_TaxCurrency;
	std::vector<C_TradePair> m_Pairs;
	std::vector<C_Tax> m_Tax;
	std::string m_ErrorMsg;
};

template<typename Fn>
void C_TaxFifo::EnumerateTradePairs(Fn & fn) const
{
	for (C_TradePair const & pair : m_Pairs)
	{
		fn(pair);
	}
}

template<typename Fn>
void C_TaxFifo::EnumerateTaxes(Fn & fn) const
{
	for (C_Tax const & tax : m_Tax)
	{
		fn(tax);
	}
}
