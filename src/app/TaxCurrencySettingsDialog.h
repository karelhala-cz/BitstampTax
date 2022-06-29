//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021-2022 Karel Hala
// License:	  MIT
//*********************************************************************************************************************

#pragma once
#include "TaxCurrencySettings.h"

#include <wx/wx.h>

class wxTextCtrl;
class wxCloseEvent;
class wxGrid;
class wxGridEvent;


class C_TaxCurrencySettingsDialog : public wxDialog
{
public:
	C_TaxCurrencySettingsDialog(C_TaxCurrencySettings &	taxCurrencySettings);

	void ReadData();
	
	void RemoveInvalidData();
	void SortData();
	void RemoveDuplicates();
	void WriteData();

	void OnKeyDown(wxKeyEvent & event);
	void OnClose(wxCloseEvent &) { this->Destroy(); } // Ask wxWidgets to `delete this; }
	void OnApply(wxCommandEvent & event);
	void OnGridChanged(wxGridEvent & event);

	bool GetGridCurrencyPairsRow(unsigned const rowIndex, C_TaxCurrencySettings::S_TaxCurrencyTableRow & row);
	void SetGridCurrencyPairsRow(unsigned const rowIndex, C_TaxCurrencySettings::S_TaxCurrencyTableRow const & row);

private:
	C_TaxCurrencySettings &	m_TaxCurrencySettings;
	
	wxTextCtrl *	m_TaxCurrencyCtrl;
	wxGrid *		m_GridCurrencyPairs;
};
