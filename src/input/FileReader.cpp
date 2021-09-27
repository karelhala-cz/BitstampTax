//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
// License:	  
//*********************************************************************************************************************

#include "FileReader.h"
#include "trade_book/TradeItemDeposit.h"
#include "trade_book/TradeItemMarket.h"
#include "currency/CurrencyValue.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <memory>


//static
char const * const C_FileReader::ITEM_NAMES[] = {
	"Type",
	"Datetime",
	"Account",
	"Amount",
	"Value",
	"Rate",
	"Fee",
	"Sub Type"
};

C_FileReader::C_FileReader(std::string const& filename)
	: m_FileName(filename)
{
	assert(static_cast<size_t>(E_Item::LastToGetCount) == std::size(ITEM_NAMES));

	for (size_t i = 0; i < std::size(ITEM_NAMES); ++i)
	{
		m_ItemNames.emplace(C_HashName(ITEM_NAMES[i]), static_cast<E_Item>(i));
	}
}

bool C_FileReader::Read()
{
	bool retVal(false);

	std::ifstream fileStream(m_FileName);
	if (fileStream)
	{
		if (ReadHeader(fileStream))
		{
			if (ReadData(fileStream))
			{
				retVal = true;
			}
		}
	}
	else
	{
		m_ErrorMsg += "Cannot open file ";
		m_ErrorMsg += m_FileName;
		m_ErrorMsg += "\n";
	}

	return retVal;
}

bool C_FileReader::ReadHeader(std::ifstream& stream)
{
	bool itemFound(false);
	bool error(false);

	std::string line;
	if (std::getline(stream, line))
	{
		std::stringstream lineStream(line);

		std::string item;
		while (std::getline(lineStream, item, ','))
		{
			C_HashName const itemHashName(item.c_str());
			T_ItemNames::const_iterator it = m_ItemNames.find(itemHashName);
			if (it != m_ItemNames.end())
			{
				m_Header.emplace_back(it->second);
				itemFound = true;
			}
			else
			{
				error = true;
				m_ErrorMsg += "Cannot read header item ";
				m_ErrorMsg += item;
				m_ErrorMsg += "\n";
				break;
			}
		}
	}
	else
	{
		error = true;
		m_ErrorMsg += "Cannot read header line\n";
	}

	return !error;
}

bool C_FileReader::ReadData(std::ifstream & stream)
{
	bool error(false);

	std::string line;
	while (std::getline(stream, line))
	{
		std::stringstream lineStream (line);

		std::string item;
		if (std::getline(lineStream, item, ','))
		{
			std::unique_ptr<C_TradeItem> tradeItem;

			if (item.compare("Deposit") == 0)
			{
				tradeItem = ReadTradeItemDeposit(lineStream);
			}
			else if (item.compare("Market") == 0)
			{
				tradeItem = ReadTradeItemMarket(lineStream);
			}
			else
			{
				error = true;
				m_ErrorMsg += "Bad data format\n";
				break;
			}

			m_TradeItems.emplace_back(std::move(tradeItem));
		}
	}

	return !error;
}

T_TradeItemUniquePtr C_FileReader::ReadTradeItemDeposit(std::stringstream & stream)
{
	bool error (false);

	std::tm tmTime;
	T_CurrencyValueConstPtr amount;

	for (size_t itemCounter = 1; itemCounter < m_Header.size(); ++itemCounter)
	{
		E_Item const itemType (m_Header.at(itemCounter));

		switch (itemType)
		{
			case E_Item::Datetime:
			{
				if (!ReadTime(stream, tmTime))
				{
					error = true;
				}
			}
			break;

			case E_Item::Account:
			{
				std::string item;
				if (!std::getline(stream, item, ','))
				{
					error = true;
				}
			}
			break;

			case E_Item::Amount:
			{
				amount = ReadCurrencyValue(stream);
				if (amount == nullptr)
				{
					error = true;
				}
			}
			break;

			case E_Item::Value:
			{

			}
			break;

			case E_Item::Rate:
			{

			}
			break;

			case E_Item::Fee:
			{

			}
			break;

			case E_Item::SubType:
			{

			}
			break;

			default:
				error = true;
				m_ErrorMsg += "Unexpected item type\n";
		}

		if (error)
		{
			break;
		}
	}

	T_TradeItemUniquePtr retVal;

	if (!error)
	{
		std::time_t time = mktime(&tmTime);
		retVal = std::make_unique<C_TradeItemDeposit>(time, std::move(amount));
	}

	return retVal;
}

T_TradeItemUniquePtr C_FileReader::ReadTradeItemMarket(std::stringstream & stream)
{
	bool error (false);

	std::time_t time (0);
	T_CurrencyValueConstPtr amount;
	T_CurrencyValueConstPtr value;
	T_CurrencyValueConstPtr rate;
	T_CurrencyValueConstPtr fee;
	E_TradeType tradeType (E_TradeType::Buy);

	for (E_Item const & item : m_Header)
	{
		if (!error)
		{
			switch (item)
			{
				case E_Item::Type:
				{
				}
				break;

				case E_Item::Datetime:
				{
					std::tm tmTime;
					if (ReadTime(stream, tmTime))
					{
						time = mktime(&tmTime);
					}
					else
					{
						error = true;
					}
				}
				break;

				case E_Item::Account:
				{
					std::string account;
					if (!std::getline(stream, account, ','))
					{
						error = true;
					}
				}
				break;

				case E_Item::Amount:
				{
					amount = ReadCurrencyValue(stream);
					if (amount == nullptr)
					{
						error = true;
					}
				}
				break;

				case E_Item::Value:
				{
					value = ReadCurrencyValue(stream);
					if (value == nullptr)
					{
						error = true;
					}
				}
				break;

				case E_Item::Rate:
				{
					rate = ReadCurrencyValue(stream);
					if (rate == nullptr)
					{
						error = true;
					}
				}
				break;

				case E_Item::Fee:
				{
					fee = ReadCurrencyValue(stream);
					if (fee == nullptr)
					{
						error = true;
					}
				}
				break;

				case E_Item::SubType:
				{
					if (!ReadTradeType(stream, tradeType))
					{
						error = true;
					}
				}
				break;

				case E_Item::LastToGetCount:
				default:
				{
					assert(false);	// Invalid item!
				}
				break;
			}
		}
	}
	return std::make_unique<C_TradeItemMarket>(time, std::move(amount), std::move(value), std::move(rate), std::move(fee), tradeType);
}

bool C_FileReader::ReadTime(std::stringstream & stream, std::tm & time)
{
	memset(&time, 0, sizeof(std::tm));

	bool error (false);

	std::string item;
	if (!std::getline(stream, item, '"'))
	{
		error = true;
	}

	if (!error && std::getline(stream, item, '.'))
	{
		if (!ReadTimeMonth(item, time.tm_mon))
		{
			error = true;
		}
	}

	if (!error && !(stream >> time.tm_mday))
	{
		error = true;
	}

	if (!error && !std::getline(stream, item, ','))
	{
		error = true;
	}

	if (!error && !(stream >> time.tm_year))
	{
		error = true;
	}
	else
	{
		time.tm_year -= 1900;
	}

	if (!error && !std::getline(stream, item, ','))
	{
		error = true;
	}

	if (!error && !(stream >> time.tm_hour))
	{
		error = true;
	}

	if (!error && !std::getline(stream, item, ':'))
	{
		error = true;
	}

	if (!error && !(stream >> time.tm_min))
	{
		error = true;
	}

	if (!error && std::getline(stream, item, '"'))
	{
		if (strcmp(item.c_str(), " PM") == 0)
		{
			time.tm_hour += 12;
		}
		else if (strcmp(item.c_str(), " AM") == 0)
		{
		}
		else
		{
			error = true;
		}
	}
	else
	{
		error = true;
	}

	if (!std::getline(stream, item, ','))
	{
		error = true;
	}

	return !error;
}

bool C_FileReader::ReadTimeMonth(std::string const & text, int & month)
{
	bool monthValid (false);

	if (text.compare("Jan") == 0)
	{
		month = 0;
		monthValid = true;
	}
	else if (text.compare("Feb") == 0)
	{
		month = 1;
		monthValid = true;
	}
	else if (text.compare("Mar") == 0)
	{
		month = 2;
		monthValid = true;
	}
	else if (text.compare("Apr") == 0)
	{
		month = 3;
		monthValid = true;
	}
	else if (text.compare("May") == 0)
	{
		month = 4;
		monthValid = true;
	}
	else if (text.compare("Jun") == 0)
	{
		month = 5;
		monthValid = true;
	}
	else if (text.compare("Jul") == 0)
	{
		month = 6;
		monthValid = true;
	}
	else if (text.compare("Aug") == 0)
	{
		month = 7;
		monthValid = true;
	}
	else if (text.compare("Sep") == 0)
	{
		month = 8;
		monthValid = true;
	}
	else if (text.compare("Oct") == 0)
	{
		month = 9;
		monthValid = true;
	}
	else if (text.compare("Nov") == 0)
	{
		month = 10;
		monthValid = true;
	}
	else if (text.compare("Dec") == 0)
	{
		month = 11;
		monthValid = true;
	}

	return monthValid;
}

T_CurrencyValueConstPtr	C_FileReader::ReadCurrencyValue(std::stringstream & stream)
{
	T_CurrencyValueConstPtr retVal;

	bool error (false);
	
	std::string valueStr;
	int64_t value (0);
	int8_t valueDecimalShift (0);
	E_CurrencyType currencyType (E_CurrencyType::Count);
	if (std::getline(stream, valueStr, ' '))
	{
		if (!valueStr.empty())
		{
			int64_t figureCount (0);
			int64_t decimalMultiplier (1);
			for (int decimalPosition = static_cast<int>(valueStr.length() - 1); decimalPosition >= 0; --decimalPosition)
			{
				char const character (valueStr.at(decimalPosition));
				if ((character >= '0') && (character <= '9'))
				{
					int64_t number (character - '0');
					if (figureCount > 0)
					{
						number *= decimalMultiplier;
					}

					value += number;
					figureCount++;
					decimalMultiplier *= 10;
				}
				else if (character == '.')
				{
					if (figureCount > 0)
					{
						assert(-figureCount > std::numeric_limits<int8_t>::min());
						valueDecimalShift = static_cast<int8_t>(-figureCount);
					}
				}
				else
				{
					error = true;
					break;
				}
			}
		}
	}
	else
	{
		error = true;
	}

	if (!error)
	{
		std::string currencyTypeStr;
		if (std::getline(stream, currencyTypeStr, ','))
		{
			if (currencyTypeStr.compare("EUR") == 0)
			{
				currencyType = E_CurrencyType::EUR;
			}
			else if (currencyTypeStr.compare("BTC") == 0)
			{
				currencyType = E_CurrencyType::BTC;
			}
			else
			{
				error = true;
			}
		}

		if (!error)
		{
			retVal = std::make_unique<C_CurrencyValue>(currencyType, value, valueDecimalShift);
		}
	}

	return std::move(retVal);
}

bool C_FileReader::ReadTradeType(std::stringstream & stream, E_TradeType & type)
{
	bool success (false);

	std::string typeText;
	if (std::getline(stream, typeText))
	{
		if (typeText.compare("Buy") == 0)
		{
			type = E_TradeType::Buy;
			success = true;
		}
		else if (typeText.compare("Sell") == 0)
		{
			type = E_TradeType::Sell;
			success = true;
		}
	}

	return success;
}