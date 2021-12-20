//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
//*********************************************************************************************************************

#pragma once
#include <wx/wx.h>


class C_FileReader;
class C_TradeBook;
class C_TaxFifo;

class wxGrid;

class C_MainFrame : public wxFrame
{
public:
    C_MainFrame();

private:
    void OnOpenFile(wxCommandEvent& event);
    void OnExit(wxCommandEvent & event);
    void OnAbout(wxCommandEvent & event);

	void OnDataChanged();
	
	void UpdateListBoxFile();
	void UpdateGridPairs();
	void UpdateGridFees();
	void UpdateGridTaxes();

	wxString GetDateTimeString(time_t const t);

private:
    wxListBox *		m_ListBoxFile;
    wxGrid *		m_GridPairs;
	wxGrid *		m_GridFees;
    wxGrid *		m_GridTaxes;

	std::unique_ptr<C_FileReader>	m_FileReader;
	std::unique_ptr<C_TradeBook>	m_TradeBook;
	std::unique_ptr<C_TaxFifo>		m_TaxFifo;
};
