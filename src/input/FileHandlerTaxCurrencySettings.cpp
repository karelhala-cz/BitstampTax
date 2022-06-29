//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021-2022 Karel Hala
// License:	  MIT
//*********************************************************************************************************************

#include "FileHandlerTaxCurrencySettings.h"
#include "app/TaxCurrencySettings.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cassert>
#include <memory>


//static
bool C_FileHandlerTaxCurrencySettings::Load(std::string const & filename, C_TaxCurrencySettings & settings)
{
	bool retVal (false);

	settings.Clear();

	std::ifstream fileStream(filename);
	if (fileStream)
	{
		retVal = ReadTaxCurrency(fileStream, settings) && ReadCurrencyRateTable(fileStream, settings);
	}
	else
	{
		std::cerr<<"Failed to open file : " << strerror(errno) <<std::endl;
	}

	return retVal;
}

//static
bool C_FileHandlerTaxCurrencySettings::Write(std::string const & filename, C_TaxCurrencySettings const & settings)
{
	bool retVal (false);

	std::ofstream fileStream (filename);
	if (fileStream.is_open())
	{
		fileStream << "\"" << settings.GetCurrencyType() << "\"";

		for (C_TaxCurrencySettings::S_TaxCurrencyTableRow const & row : settings.GetTable())
		{
			fileStream << std::endl << row;
		}
		
		fileStream.close();
		if (!fileStream.is_open())
		{
			retVal = true;
		}
		else
		{
			std::cerr<<"Failed to close file : " << strerror(errno) <<std::endl;
		}
	}
	else
	{
		std::cerr<<"Failed to open file : " << strerror(errno) <<std::endl;
	}

	return true;
}

//static
bool C_FileHandlerTaxCurrencySettings::ReadTaxCurrency(std::ifstream & fileStream, C_TaxCurrencySettings & settings)
{
	bool retVal (true);

	std::string taxCurrency;
	fileStream >> std::quoted(taxCurrency);
	settings.SetCurrencyType(taxCurrency);

	return retVal;
}

//static
bool C_FileHandlerTaxCurrencySettings::ReadCurrencyRateTable(std::ifstream & fileStream, C_TaxCurrencySettings & settings)
{
	bool retVal (true);

	C_TaxCurrencySettings::S_TaxCurrencyTableRow row;
	while (fileStream >> row)
	{
		settings.AddTableRow(row);
	}

	return retVal;
}

//static
bool C_FileHandlerTaxCurrencySettings::ReadQuotedString(std::ifstream & fileStream, std::string & outStr)
{
	bool retVal (false);

	std::string item;
	if (std::getline(fileStream, item, '"'))
	{
		retVal = true;
	}

	return retVal;
}