//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021-2022 Karel Hala
// License:	  MIT
//*********************************************************************************************************************

#include "Tax.h"

C_Tax::C_Tax(T_Year const year, T_CurrencyType const & currencyType, T_CurrencyType const & taxCurrencyType)
	: m_Year(year)
	, m_Expenditures(currencyType)
	, m_Receipts(currencyType)
	, m_ExpendituresInTaxCurrency(taxCurrencyType)
	, m_ReceiptsInTaxCurrency(taxCurrencyType)
{
}

void C_Tax::AddExpenditure(C_CurrencyValue const & value, C_CurrencyValue const & valueInTaxCurrency)
{
	m_Expenditures += value;
	m_ExpendituresInTaxCurrency += valueInTaxCurrency;
}

void C_Tax::AddReceipt(C_CurrencyValue const & value, C_CurrencyValue const & valueInTaxCurrency)
{
	m_Receipts += value;
	m_ReceiptsInTaxCurrency += valueInTaxCurrency;
}
