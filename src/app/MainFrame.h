//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021-2022 Karel Hala
// License:	  MIT
//*********************************************************************************************************************

#pragma once
#include <wx/wx.h>

#include <filesystem>


class C_TaxCurrencySettings;
class C_FileReader;
class C_TradeBook;
class C_TaxFifo;
class C_Decimal;
class C_CurrencyValue;

class wxGrid;

class C_MainFrame : public wxFrame
{
	static char const * const FILE_NAME_TAX_CURRENCY_SETTINGS;

	static bool EnsureDirExists(std::filesystem::path const & dir);	
	static std::string GetTaxCurrencySettingsDefaultFilePath();

	enum class E_GridPairsCollType
	{
		SellDatetime,
		SellPrice,
		Amount,
		BuyDatetime,
		BuyPrice,
		Proceeds,
		Cost,
		PnL,
		SellCurrencyRate,
		ProceedsInTaxCurrency,
		BuyCurrencyRate,
		CostInTaxCurrency,
		PnLInTaxCurrency,
		_Count
	};

	enum class E_GridFeesCollType
	{
		Datetime,
		Fee,
		FeeCurrencyRate,
		FeeInTaxCurrency,
		_Count
	};

	enum class E_GridTaxesCollType
	{
		Year,
		Expenditures,
		Receipts,
		PnL,
		ExpendituresInTaxCurrency,
		ReceiptsInTaxCurrency,
		PnLInTaxCurrency,
		_Count
	};

public:
    C_MainFrame();

private:
    void OnOpenFile(wxCommandEvent& event);
	void OnTaxCurrencySettings(wxCommandEvent& event);
    void OnExit(wxCommandEvent & event);
    void OnAbout(wxCommandEvent & event);
	void OnListBoxFile(wxCommandEvent & event);

	void OnDataChanged();
	void OnSelectedFileLine(size_t const inputFileLine);
	
	void UpdateListBoxFile();
	void UpdateGridPairs();
	void UpdateGridFees();
	void UpdateGridTaxes();

	void UpdateGridPairsSelection(size_t const selectedFileLine);
	void UpdateGridFeesSelection(size_t const selectedFileLine);

	wxString GetDateTimeString(time_t const t);

	void LoadTaxCurrencySettings();
	void SaveTaxCurrencySettings();

	C_CurrencyValue ConvertToTaxCurrency(C_CurrencyValue const & value, time_t const time) const;
	C_CurrencyValue ConvertToTaxCurrency(C_CurrencyValue const & value, time_t const time, C_Decimal & rate) const;

private:
    wxListBox *		m_ListBoxFile;
    wxGrid *		m_GridPairs;
	wxGrid *		m_GridFees;
    wxGrid *		m_GridTaxes;

	std::unique_ptr<C_TaxCurrencySettings>	m_TaxCurrencySettings;
	std::unique_ptr<C_FileReader>			m_FileReader;
	std::unique_ptr<C_TradeBook>			m_TradeBook;
	std::unique_ptr<C_TaxFifo>				m_TaxFifo;
};
