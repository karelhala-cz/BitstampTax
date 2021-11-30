//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
// License:	  
//*********************************************************************************************************************

#include "Tax.h"

C_Tax::C_Tax(T_Year const year, T_CurrencyType const & currencyType)
	: m_Year(year)
	, m_Expenditures(currencyType)
	, m_Receipts(currencyType)
{
}

void C_Tax::AddExpenditure(C_CurrencyValue const & value)
{
	m_Expenditures += value;
}

void C_Tax::AddReceipt(C_CurrencyValue const & value)
{
	m_Receipts += value;
}
