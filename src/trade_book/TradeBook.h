//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021-2022 Karel Hala
// License:	  MIT
//*********************************************************************************************************************

#pragma once

#include "TradeItemFwd.h"
#include <currency/CurrencyType.h>
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

	template<typename Fn>
	void EnumerateTradeItemsMarket(Fn & fn) const;

	bool IsEmpty() const { return m_TradeItems.empty(); }
	T_TradeItems const & GetData() const { return m_TradeItems; }

	T_CurrencyType AssessUserCurrency() const;

	void PrintData(std::ostringstream & str);

private:
	T_TradeItems m_TradeItems;
};


template<typename Fn>
void C_TradeBook::EnumerateTradeItemsMarket(Fn & fn) const
{
	for (T_TradeItemUniquePtr const & item : GetData())
	{
		C_TradeItemMarket const * const itemMarket (dynamic_cast<C_TradeItemMarket const *>(item.get()));
		if (itemMarket != nullptr)
		{
			fn(*itemMarket);
		}
	}
}
