//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
// License:	  
//*********************************************************************************************************************

#include "TradeBook.h"
#include "TradeItem.h"

C_TradeBook::C_TradeBook()
{
}

void C_TradeBook::Reserve(size_t const count)
{
	m_TradeItems.reserve(count);
}

void C_TradeBook::AddItem(std::unique_ptr<C_TradeItem> && item)
{
	m_TradeItems.emplace_back(std::move(item));
}

void C_TradeBook::SetTradeItems(T_TradeItems && items)
{
	m_TradeItems = std::move(items);
}

void C_TradeBook::PrintData(std::ostringstream & str)
{
	for (T_TradeItemUniquePtr & item : m_TradeItems)
	{
		item->PrintData(str);
		str << std::endl;
	}
}