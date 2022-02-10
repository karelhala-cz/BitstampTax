//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021-2022 Karel Hala
// License:	  MIT
//*********************************************************************************************************************

#include "TaxFifo.h"
#include "trade_book/TradeItemMarket.h"
#include <map>
#include <iostream>
#include <cassert>

C_TaxFifo::C_TaxFifo(T_CurrencyType const & taxCurrency)
	: m_TaxCurrency(taxCurrency)
{
}

bool C_TaxFifo::Process(C_TradeBook const & book)
{
	std::vector<C_TradeItemMarket const *>	buys;
	std::vector<C_TradeItemMarket const *>	sells;

	for (T_TradeItemUniquePtr const & item : book.GetData())
	{
		C_TradeItemMarket const * const itemMarket (dynamic_cast<C_TradeItemMarket const *>(item.get()));
		if (itemMarket != nullptr)
		{
			C_Tax & taxYear(GetTaxYear(GetYear(itemMarket->GetTime())));

			if (itemMarket->IsFeeValid())
			{
				taxYear.AddExpenditure(itemMarket->GetFee());
			}

			switch (itemMarket->GetTradeType())
			{
				case E_TradeType::Buy:
				{
					buys.push_back(itemMarket);
				}
				break;

				case E_TradeType::Sell:
				{
					sells.push_back(itemMarket);
				}
				break;

				default:
					assert(0);
			}
		}
	}

	bool error (false);
	if (FindPairs(buys, sells))
	{
		ProcessPairs();
	}
	else
	{
		error = true;
	}

	return !error;
}

C_Tax::T_Year C_TaxFifo::GetYear(std::time_t const time)
{
	tm utcTime;
	localtime_s(&utcTime, &time);

	C_Tax::T_Year const tradeYear(1900 + utcTime.tm_year);

	return tradeYear;
}

C_Tax & C_TaxFifo::GetTaxYear(C_Tax::T_Year const year)
{
	for (C_Tax & taxYear : m_Tax)
	{
		if (year == taxYear.GetYear())
		{
			return taxYear;
		}
	}

	m_Tax.emplace_back(year, m_TaxCurrency);

	return m_Tax.back();
}

bool C_TaxFifo::FindPairs(T_TradeItems const & buys, T_TradeItems const & sells)
{
	bool error(false);

	if (!buys.empty() && !sells.empty())
	{
		struct S_BuyItem
		{
			S_BuyItem(size_t const index, C_CurrencyValue const & amount)
				: m_Index(index)
				, m_BoughtAmountToProcess(amount)
			{
			}

			size_t			m_Index;
			C_CurrencyValue m_BoughtAmountToProcess;
		};

		typedef std::map<T_CurrencyType, S_BuyItem> T_BuyItemPerType;
		T_BuyItemPerType buyItemPerType;

		for (C_TradeItemMarket const * const sell : sells)
		{
			C_CurrencyValue sellAmountToProcess(sell->GetAmount());

			T_BuyItemPerType::iterator itBuyItemPerType = buyItemPerType.find(sellAmountToProcess.GetType());
			if (itBuyItemPerType == buyItemPerType.end())
			{
				size_t const buyItemIndex(GetFirstItemIndexForType(sellAmountToProcess.GetType(), sell->GetTime(), buys));
				if ((buyItemIndex < buys.size()))
				{
					C_CurrencyValue const & buyAmount(buys.at(buyItemIndex)->GetAmount());
					std::pair<T_BuyItemPerType::iterator, bool> ret(buyItemPerType.insert(std::pair<T_CurrencyType, S_BuyItem>(buyAmount.GetType(), S_BuyItem(buyItemIndex, buyAmount))));
					assert(ret.second == true);	//item inserted
					itBuyItemPerType = ret.first;
				}
			}
			
			if (itBuyItemPerType == buyItemPerType.end())
			{
				error = true;
				m_ErrorMsg += "Can not assess buy-sell pairs!\n";
				break;
			}

			S_BuyItem & buyItemCache(itBuyItemPerType->second);

			do 
			{
				if (sellAmountToProcess < buyItemCache.m_BoughtAmountToProcess)
				{
					AddPair(sellAmountToProcess, *buys[buyItemCache.m_Index], *sell);
					buyItemCache.m_BoughtAmountToProcess -= sellAmountToProcess;
					sellAmountToProcess.Zero();
				}
				else if (sellAmountToProcess > buyItemCache.m_BoughtAmountToProcess)
				{
					AddPair(buyItemCache.m_BoughtAmountToProcess, *buys[buyItemCache.m_Index], *sell);
					sellAmountToProcess -= buyItemCache.m_BoughtAmountToProcess;
					buyItemCache.m_BoughtAmountToProcess.Zero();
				}
				else
				{
					AddPair(sellAmountToProcess, *buys[buyItemCache.m_Index], *sell);
					buyItemCache.m_BoughtAmountToProcess.Zero();
					sellAmountToProcess.Zero();
				}

				if (buyItemCache.m_BoughtAmountToProcess.IsZero())
				{
					size_t const nextBuyIndex(GetNextItemIndexForType(sellAmountToProcess.GetType(), sell->GetTime(), buyItemCache.m_Index, buys));
					if (nextBuyIndex < buys.size())
					{
						buyItemCache.m_Index = nextBuyIndex;
						buyItemCache.m_BoughtAmountToProcess = buys[nextBuyIndex]->GetAmount();
					}
					else
					{
						if (!sellAmountToProcess.IsZero())
						{
							error = true;
							m_ErrorMsg += "Can not assess buy-sell pairs!\n";
						}
					}
				}
			} while (!sellAmountToProcess.IsZero() && (buyItemCache.m_Index < buys.size()) && !error);

			if (error)
			{
				break;
			}
		}
	}

	return !error;
}

size_t C_TaxFifo::GetFirstItemIndexForType(T_CurrencyType const & type, std::time_t const maxTime, T_TradeItems const & items) const
{
	size_t retIndex(0);
	
	for (; retIndex < items.size(); ++retIndex)
	{
		C_TradeItemMarket const * const item(items.at(retIndex));
		if (item->GetTime() > maxTime)
		{
			retIndex = items.size();
			break;
		}

		if (type == item->GetAmount().GetType())
		{
			break;
		}
	}

	return retIndex;
}

size_t C_TaxFifo::GetNextItemIndexForType(T_CurrencyType const & type, std::time_t const maxTime, size_t const index, T_TradeItems const & items) const
{
	size_t retIndex(index + 1);
	
	for (; retIndex < items.size(); ++retIndex)
	{
		C_TradeItemMarket const * const item(items.at(retIndex));
		if (item->GetTime() > maxTime)
		{
			retIndex = items.size();
			break;
		}

		if (type == items.at(retIndex)->GetAmount().GetType())
		{
			break;
		}
	}

	return retIndex;
}

void C_TaxFifo::AddPair(C_CurrencyValue const & amount, C_TradeItemMarket const & buyItem, C_TradeItemMarket const & sellItem)
{
	m_Pairs.emplace_back(amount,
		buyItem.GetInputFileLine(),
		buyItem.GetRate(),
		buyItem.GetTime(),
		sellItem.GetInputFileLine(),
		sellItem.GetRate(),
		sellItem.GetTime());
}

void C_TaxFifo::ProcessPairs()
{
	for (C_TradePair const & pair : m_Pairs)
	{
		C_Tax & taxYear(GetTaxYear(GetYear(pair.GetSellTime())));

		taxYear.AddExpenditure(pair.GetBuyPrice() * pair.GetAmount().GetValue());
		taxYear.AddReceipt(pair.GetSellPrice() * pair.GetAmount().GetValue());
	}
}

void C_TaxFifo::PrintTax(std::ostringstream & str)
{
	for (C_Tax const & taxYear : m_Tax)
	{
		str << "Tax year: " << taxYear.GetYear();
		
		str << ", expeditures: ";
		taxYear.GetExpenditures().PrintData(str);

		str << ", receipts: ";
		taxYear.GetReceipts().PrintData(str);

		str << ", profit: ";
		taxYear.GetProfit().PrintData(str);

		str << "\n";
	}
}
