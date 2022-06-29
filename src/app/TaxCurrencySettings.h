//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021-2022 Karel Hala
// License:	  MIT
//*********************************************************************************************************************

#pragma once

#include "currency/CurrencyType.h"
#include "types/Decimal.h"

#include <vector>

class C_CurrencyValue;


class C_TaxCurrencySettings
{
public:
	struct S_TaxCurrencyTableFindParams
	{
		S_TaxCurrencyTableFindParams(T_CurrencyType base, T_CurrencyType quote, unsigned year);

		T_CurrencyType m_Base;
		T_CurrencyType m_Quote;
		unsigned m_Year;
	};

	struct S_TaxCurrencyTableRow
	{
		enum E_CollType { Base, Quote, Year, Rate, _Count };

		S_TaxCurrencyTableRow();
		S_TaxCurrencyTableRow(T_CurrencyType base, T_CurrencyType quote, unsigned year, C_Decimal const & rate);

		bool IsValid() const;

		C_CurrencyValue Convert(C_CurrencyValue const & source) const;

		friend std::istream & operator>> (std::istream & in, S_TaxCurrencyTableRow & row);
		friend std::ostream & operator<< (std::ostream & out, S_TaxCurrencyTableRow const & row);

		bool operator < (S_TaxCurrencyTableRow const & other) const;
		bool operator == (S_TaxCurrencyTableFindParams const & other) const;

		T_CurrencyType m_Base;
		T_CurrencyType m_Quote;
		unsigned m_Year;
		C_Decimal m_Rate;
	};

	C_TaxCurrencySettings();

	void Clear();

	T_CurrencyType GetCurrencyType() const { return m_CurrencyType; }
	std::vector<S_TaxCurrencyTableRow> const & GetTable() const { return m_Table; }

	void SetCurrencyType(T_CurrencyType const & currencyType);
	void AddTableRow(S_TaxCurrencyTableRow const & row);
	void AddTableRow(T_CurrencyType base, T_CurrencyType quote, unsigned year, C_Decimal const & rate);

	void LoadFile(std::string const & filename);
	void WriteFile(std::string const & filename);

	C_CurrencyValue ConvertToTaxCurrency(C_CurrencyValue const & source, unsigned const year) const;
	C_CurrencyValue ConvertToTaxCurrency(C_CurrencyValue const & source, unsigned const year, C_Decimal & rate) const;

private:
	T_CurrencyType						m_CurrencyType;
	std::vector<S_TaxCurrencyTableRow>	m_Table;
};
