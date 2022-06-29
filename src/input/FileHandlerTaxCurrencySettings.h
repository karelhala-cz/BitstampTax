//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021-2022 Karel Hala
// License:	  MIT
//*********************************************************************************************************************

#pragma once

#include <string>
#include <iosfwd>

class C_TaxCurrencySettings;


class C_FileHandlerTaxCurrencySettings
{
public:
	static bool Load(std::string const & filename, C_TaxCurrencySettings & settings);
	static bool Write(std::string const & filename, C_TaxCurrencySettings const & settings);

private:
	static bool ReadTaxCurrency(std::ifstream & fileStream, C_TaxCurrencySettings & settings);
	static bool ReadCurrencyRateTable(std::ifstream & fileStream, C_TaxCurrencySettings & settings);
	static bool ReadQuotedString(std::ifstream & fileStream, std::string & outStr);
};
