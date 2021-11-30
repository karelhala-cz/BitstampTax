//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
// License:	  
//*********************************************************************************************************************

#include "TradeBook.h"
#include "TradeItem.h"
#include "TradeItemMarket.h"

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

T_CurrencyType C_TradeBook::AssessUserCurrency() const
{
	T_CurrencyType userCurrency;

	for (T_TradeItemUniquePtr const & item : m_TradeItems)
	{
		C_TradeItemMarket const * const itemMarket (dynamic_cast<C_TradeItemMarket const *>(item.get()));
		if (itemMarket != nullptr)
		{
			userCurrency = itemMarket->GetValue().GetType();
			break;
		}
	}

	return userCurrency;
}

void C_TradeBook::PrintData(std::ostringstream & str)
{
	for (T_TradeItemUniquePtr & item : m_TradeItems)
	{
		item->PrintData(str);
		str << std::endl;
	}
}