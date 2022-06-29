//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021-2022 Karel Hala
// License:	  MIT
//*********************************************************************************************************************

#include "TaxCurrencySettings.h"
#include "input/FileHandlerTaxCurrencySettings.h"
#include "currency/CurrencyValue.h"

#include <iomanip>
#include <algorithm>

#include <assert.h> 

C_TaxCurrencySettings::S_TaxCurrencyTableFindParams::S_TaxCurrencyTableFindParams(T_CurrencyType base, T_CurrencyType quote, unsigned year)
	: m_Base(base)
	, m_Quote(quote)
	, m_Year(year)
{
}



C_TaxCurrencySettings::S_TaxCurrencyTableRow::S_TaxCurrencyTableRow()
	: m_Base()
	, m_Quote()
	, m_Year(0)
	, m_Rate()
{

}

C_TaxCurrencySettings::S_TaxCurrencyTableRow::S_TaxCurrencyTableRow(T_CurrencyType base, T_CurrencyType quote, unsigned year, C_Decimal const & rate)
	: m_Base(base)
	, m_Quote(quote)
	, m_Year(year)
	, m_Rate(rate)
{
}

bool C_TaxCurrencySettings::S_TaxCurrencyTableRow::IsValid() const
{
	return (m_Base.length() > 0) &&
		(m_Quote.length() > 0) &&
		(m_Year > 0) &&
		!m_Rate.IsZero();
}

C_CurrencyValue C_TaxCurrencySettings::S_TaxCurrencyTableRow::Convert(C_CurrencyValue const & source) const
{
	assert(IsValid());
	assert(source.GetType() == m_Base);
	assert(source.GetValue().IsValid());

	return C_CurrencyValue(m_Quote, source.GetValue() * m_Rate);
}

std::istream & operator>> (std::istream & in, C_TaxCurrencySettings::S_TaxCurrencyTableRow & row)
{
	in >> std::quoted(row.m_Base) >> std::quoted(row.m_Quote) >> row.m_Year >> row.m_Rate;

	return in;
}

std::ostream & operator<< (std::ostream & out, C_TaxCurrencySettings::S_TaxCurrencyTableRow const & row)
{
	char const * const delimiter = " ";
	
	out << std::quoted(row.m_Base)
		<< delimiter << std::quoted(row.m_Quote)
		<< delimiter << row.m_Year
		<< delimiter << row.m_Rate;

	return out;
}

bool C_TaxCurrencySettings::S_TaxCurrencyTableRow::operator < (C_TaxCurrencySettings::S_TaxCurrencyTableRow const & other) const
{
	bool retVal (false);
	
	assert (IsValid() && other.IsValid());

	if (m_Base < other.m_Base)
	{
		retVal = true;
	}
	else if (m_Base == other.m_Base)
	{
		if (m_Quote < other.m_Quote)
		{
			retVal = true;
		}
		else if (m_Quote == other.m_Quote)
		{
			if (m_Year < other.m_Year)
			{
				retVal = true;
			}
		}
	}

	return retVal;
}

bool C_TaxCurrencySettings::S_TaxCurrencyTableRow::operator == (C_TaxCurrencySettings::S_TaxCurrencyTableFindParams const & other) const
{
	assert (IsValid());
	
	return ((m_Base == other.m_Base) && (m_Quote == other.m_Quote) && (m_Year == other.m_Year));
}

C_TaxCurrencySettings::C_TaxCurrencySettings()
{
	
}

void C_TaxCurrencySettings::Clear()
{
	m_CurrencyType.clear();
	m_Table.clear();
}

void C_TaxCurrencySettings::SetCurrencyType(T_CurrencyType const & currencyType)
{
	m_CurrencyType = currencyType;
}

void C_TaxCurrencySettings::AddTableRow(S_TaxCurrencyTableRow const & row)
{
	m_Table.emplace_back(row);
}

void C_TaxCurrencySettings::AddTableRow(T_CurrencyType base, T_CurrencyType quote, unsigned year, C_Decimal const & rate)
{
	m_Table.emplace_back(base, quote, year, rate);
}

void C_TaxCurrencySettings::LoadFile(std::string const & filename)
{
	C_FileHandlerTaxCurrencySettings::Load(filename, *this);
}

void C_TaxCurrencySettings::WriteFile(std::string const & filename)
{
	C_FileHandlerTaxCurrencySettings::Write(filename, *this);
}

C_CurrencyValue C_TaxCurrencySettings::ConvertToTaxCurrency(C_CurrencyValue const & source, unsigned const year) const
{
	C_Decimal rate;
	C_CurrencyValue retVal = ConvertToTaxCurrency(source, year, rate);

	return retVal;
}

C_CurrencyValue C_TaxCurrencySettings::ConvertToTaxCurrency(C_CurrencyValue const & source, unsigned const year, C_Decimal & rate) const
{
	C_CurrencyValue retVal;

	if (!GetCurrencyType().empty())
	{
		S_TaxCurrencyTableFindParams const testRow (source.GetType(), GetCurrencyType(), year);

		std::vector<S_TaxCurrencyTableRow>::const_iterator it = std::find(m_Table.begin(), m_Table.end(), testRow);

		if (it != m_Table.end())
		{
			rate = it->m_Rate;
			retVal = it->Convert(source);
		}
	}

	return retVal;
}
