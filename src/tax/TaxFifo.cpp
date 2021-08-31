//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
// License:	  
//*********************************************************************************************************************

#include "TaxFifo.h"
#include "trade_book/TradeItemMarket.h"
#include <cassert>

C_TaxFifo::C_TaxFifo()
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

			taxYear.AddExpenditure(itemMarket->GetFee());

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

	m_Tax.emplace_back(year);

	return m_Tax.back();
}

bool C_TaxFifo::FindPairs(T_TradeItems const & buys, T_TradeItems const & sells)
{
	bool error(false);

	if (!buys.empty() && !sells.empty())
	{
		T_TradeItems::const_iterator buyIt(buys.begin());
		C_CurrencyValue boughtAmountToProcess((*buyIt)->GetAmount());
		for (C_TradeItemMarket const * const sell : sells)
		{
			C_CurrencyValue sellAmountToProcess(sell->GetAmount());

			do 
			{
				if (sell->GetTime() >= (*buyIt)->GetTime())
				{
					if (sellAmountToProcess <= boughtAmountToProcess)
					{
						AddPair(sellAmountToProcess, *(*buyIt), *sell);
						boughtAmountToProcess -= sellAmountToProcess;
						break;
					}
					else
					{
						AddPair(boughtAmountToProcess, *(*buyIt), *sell);
						sellAmountToProcess -= boughtAmountToProcess;
						boughtAmountToProcess.Zero();

						buyIt++;
						boughtAmountToProcess = (*buyIt)->GetAmount();
					}
				}
				else
				{
					error = true;
					m_ErrorMsg += "Buy/Sell time mismatch!\n";
					break;
				}

			} while (buyIt < buys.end());

			if (error)
			{
				break;
			}
		}
	}

	return !error;
}

void C_TaxFifo::AddPair(C_CurrencyValue const & amount, C_TradeItemMarket const & buyItem, C_TradeItemMarket const & sellItem)
{
	m_Pairs.emplace_back(amount, buyItem.GetRate(), buyItem.GetTime(), sellItem.GetRate(), sellItem.GetTime());
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
