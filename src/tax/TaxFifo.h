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


class C_TaxFifo
{
	typedef std::vector<C_TradeItemMarket const *> T_TradeItems;

public: 
	C_TaxFifo();

	bool Process(C_TradeBook const & book);

	void PrintTax(std::ostringstream & str);

	std::string const & GetErrorMsg() const { return m_ErrorMsg; }

private:
	C_Tax::T_Year GetYear(std::time_t const time);
	C_Tax &	GetTaxYear(C_Tax::T_Year const year);

	bool FindPairs(T_TradeItems const & buys, T_TradeItems const & sells);
	void AddPair(C_CurrencyValue const & amount, C_TradeItemMarket const & buyItem, C_TradeItemMarket const & sellItem);

	void ProcessPairs();

private:
	std::vector<C_TradePair> m_Pairs;
	std::vector<C_Tax> m_Tax;
	std::string m_ErrorMsg;
};
