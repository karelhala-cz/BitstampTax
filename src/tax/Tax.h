//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
// License:	  
//*********************************************************************************************************************

#pragma once

#include "Currency/CurrencyValue.h"

class C_Tax
{
public:
	typedef int T_Year;

	C_Tax(T_Year const year, T_CurrencyType const & currencyType);

	void AddExpenditure(C_CurrencyValue const & value);
	void AddReceipt(C_CurrencyValue const & value);

	T_Year					GetYear() const { return m_Year; }
	C_CurrencyValue	const & GetExpenditures() const { return m_Expenditures; }
	C_CurrencyValue	const & GetReceipts() const { return m_Receipts; }

	C_CurrencyValue GetProfit() const { return m_Receipts - m_Expenditures; }

private:
	T_Year			m_Year;
	C_CurrencyValue	m_Expenditures;
	C_CurrencyValue	m_Receipts;
};
