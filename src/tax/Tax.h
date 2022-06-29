//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021-2022 Karel Hala
// License:	  MIT
//*********************************************************************************************************************

#pragma once

#include "Currency/CurrencyValue.h"

class C_Tax
{
public:
	typedef int T_Year;

	C_Tax(T_Year const year, T_CurrencyType const & currencyType, T_CurrencyType const & taxCurrencyType);

	void AddExpenditure(C_CurrencyValue const & value, C_CurrencyValue const & valueInTaxCurrency);
	void AddReceipt(C_CurrencyValue const & value, C_CurrencyValue const & valueInTaxCurrency);

	T_Year					GetYear() const { return m_Year; }
	
	C_CurrencyValue	const & GetExpenditures() const { return m_Expenditures; }
	C_CurrencyValue	const & GetReceipts() const { return m_Receipts; }
	C_CurrencyValue			GetProfit() const { return m_Receipts - m_Expenditures; }

	C_CurrencyValue	const & GetExpendituresInTaxCurrency() const { return m_ExpendituresInTaxCurrency; }
	C_CurrencyValue	const & GetReceiptsInTaxCurrency() const { return m_ReceiptsInTaxCurrency; }
	C_CurrencyValue			GetProfitInTaxCurrency() const { return m_ReceiptsInTaxCurrency - m_ExpendituresInTaxCurrency; }

private:
	T_Year			m_Year;

	C_CurrencyValue	m_Expenditures;
	C_CurrencyValue	m_Receipts;

	C_CurrencyValue	m_ExpendituresInTaxCurrency;
	C_CurrencyValue	m_ReceiptsInTaxCurrency;
};
