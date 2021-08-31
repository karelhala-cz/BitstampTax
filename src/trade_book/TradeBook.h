//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
// License:	  
//*********************************************************************************************************************

#pragma once

#include "TradeItemFwd.h"
#include <vector>
#include <memory>

class C_TradeBook
{

	typedef std::vector<T_TradeItemUniquePtr> T_TradeItems;

public:
	C_TradeBook();

	void Reserve(size_t const count);
	void AddItem(std::unique_ptr<C_TradeItem> && item);
	void SetTradeItems(T_TradeItems && items);

	T_TradeItems const & GetData() const { return m_TradeItems; }
	void PrintData(std::ostringstream & str);

private:
	T_TradeItems m_TradeItems;
};
