//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
// License:	  
//*********************************************************************************************************************

#pragma once
#include "utils/HashName.h"
#include "trade_book/TradeItem.h"
#include <string>
#include <map>
#include <vector>
#include <memory>

class C_FileReader
{
	enum class E_Item
	{
		Type,
		Datetime,
		Account,
		Amount,
		Value,
		Rate,
		Fee,
		SubType,

		LastToGetCount
	};

	static char const * const ITEM_NAMES[];

	typedef std::map<C_HashName, E_Item> T_ItemNames;
	typedef std::vector<E_Item> T_Header;
	typedef std::vector<T_TradeItemUniquePtr> T_TradeItems;

public:
	C_FileReader(std::string const & filename);

	bool Read();

	std::string const & GetErrorMsg() const { return m_ErrorMsg; }

	T_TradeItems && MoveTradeItems() { return std::move(m_TradeItems); }

private:
	bool ReadHeader(std::ifstream & stream);
	bool ReadData(std::ifstream & stream);
	T_TradeItemUniquePtr	ReadTradeItemDeposit(std::stringstream & stream);
	T_TradeItemUniquePtr	ReadTradeItemMarket(std::stringstream & stream);
	bool					ReadTime(std::stringstream & stream, std::tm & time);
	bool					ReadTimeMonth(std::string const & text, int & month);
	T_CurrencyValueConstPtr	ReadCurrencyValue(std::stringstream & stream);
	bool					ReadTradeType(std::stringstream & stream, E_TradeType & type);

private:
	std::string m_FileName;
	std::string m_ErrorMsg;

	T_ItemNames m_ItemNames;
	T_Header	m_Header;

	T_TradeItems m_TradeItems;
};