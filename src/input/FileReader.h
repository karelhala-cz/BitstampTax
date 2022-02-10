//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021-2022 Karel Hala
// License:	  MIT
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

	typedef std::vector<std::string> T_FileLines;
	typedef std::map<C_HashName, E_Item> T_ItemNames;
	typedef std::vector<E_Item> T_Header;
	typedef std::vector<T_TradeItemUniquePtr> T_TradeItems;

public:
	C_FileReader(std::string const & filename);

	bool Read();

	std::string const & GetErrorMsg() const { return m_ErrorMsg; }

	template<typename Fn>
	void EnumerateLines(Fn & fn) const;

	T_TradeItems && MoveTradeItems() { return std::move(m_TradeItems); }

private:
	void ReadFileLines(std::ifstream & stream);

	bool ReadHeaderLine();
	bool ReadDataLine(size_t const lineIndex);
	T_TradeItemUniquePtr	ReadTradeItemDeposit(size_t const inputFileLine, std::stringstream & stream);
	T_TradeItemUniquePtr	ReadTradeItemWithdrawal(size_t const inputFileLine, std::stringstream & stream);
	T_TradeItemUniquePtr	ReadTradeItemMarket(size_t const inputFileLine, std::stringstream & stream);
	bool					ReadTime(std::stringstream & stream, std::tm & time);
	bool					ReadTimeMonth(std::string const & text, int & month);
	T_CurrencyValueConstPtr	ReadCurrencyValue(std::stringstream & stream);
	bool					ReadTradeType(std::stringstream & stream, E_TradeType & type);
	bool					ReadEmptyString(std::stringstream & stream);

private:
	std::string m_FileName;
	T_FileLines	m_FileLines;

	std::string m_ErrorMsg;

	T_ItemNames m_ItemNames;
	T_Header	m_Header;

	T_TradeItems m_TradeItems;
};

template<typename Fn>
void C_FileReader::EnumerateLines(Fn & fn) const
{
	for (std::string const & line : m_FileLines)
	{
		fn(line);
	}
}
