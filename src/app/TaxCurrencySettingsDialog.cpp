//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021-2022 Karel Hala
// License:	  MIT
//*********************************************************************************************************************

#include "TaxCurrencySettingsDialog.h"
#include "TaxCurrencySettings.h"
#include "Ids.h"

#include <wx/grid.h>

#include <math.h> 
#include <string>


C_TaxCurrencySettingsDialog::C_TaxCurrencySettingsDialog(C_TaxCurrencySettings & taxCurrencySettings)
	: wxDialog(nullptr, ID_TaxCurrencySettings, "Tax Currency Settings", wxDefaultPosition, wxDefaultSize/*, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER */)
	, m_TaxCurrencySettings(taxCurrencySettings)
	, m_TaxCurrencyCtrl(nullptr)
	, m_GridCurrencyPairs(nullptr)
{
	wxBoxSizer * const topSizer = new wxBoxSizer(wxVERTICAL);

	static int const BORDER_SMALL(5);
	static int const BORDER_BIG(20);
	static int const PROPORTION(0); //0 for unstretchable

	wxBoxSizer * const firstLine = new wxBoxSizer(wxHORIZONTAL);

	wxString const strTaxCurrency(wxT("Tax Currency:"));
	wxStaticText * const taxCurrency (new wxStaticText(this, wxID_ANY, strTaxCurrency, wxDefaultPosition, wxDefaultSize));
	wxFont captionFont (taxCurrency->GetFont());
	captionFont.SetWeight(wxFONTWEIGHT_BOLD);
	taxCurrency->SetFont(captionFont);
	firstLine->Add(taxCurrency, PROPORTION, wxEXPAND | wxALL, BORDER_SMALL);

	wxString const strDefaultTaxCurrency(wxT(""));
	m_TaxCurrencyCtrl = new wxTextCtrl(this, wxID_ANY, strDefaultTaxCurrency, wxDefaultPosition, wxDefaultSize);
	wxSize const taxCurrencyTextSize(m_TaxCurrencyCtrl->GetSizeFromTextSize(m_TaxCurrencyCtrl->GetTextExtent("WWW")));
	m_TaxCurrencyCtrl->SetInitialSize(taxCurrencyTextSize);
	firstLine->Add(m_TaxCurrencyCtrl, PROPORTION, wxEXPAND | wxALL, BORDER_SMALL);

	topSizer->Add(firstLine, PROPORTION, wxALIGN_LEFT | wxTOP | wxBOTTOM, BORDER_BIG);

	wxBoxSizer * currencyPairsGridSizer = new wxBoxSizer(wxVERTICAL);
	wxStaticText * const captionCurrencyPairs (new wxStaticText(this, wxID_ANY, wxT("Currency Pairs:")));
	captionCurrencyPairs->SetFont(captionFont);

	currencyPairsGridSizer->Add(captionCurrencyPairs, 0, wxALIGN_LEFT);
    m_GridCurrencyPairs = new wxGrid(this, ID_PairsGrid, wxPoint(0, 0), wxDefaultSize);
	m_GridCurrencyPairs->SetCellHighlightColour(*wxBLACK);
	m_GridCurrencyPairs->EnableEditing(true);
	m_GridCurrencyPairs->SetRowLabelSize(0);	// Hide row label
	m_GridCurrencyPairs->ShowScrollbars(wxScrollbarVisibility::wxSHOW_SB_NEVER, wxScrollbarVisibility::wxSHOW_SB_ALWAYS);
	m_GridCurrencyPairs->SetDefaultCellAlignment(wxALIGN_RIGHT, wxALIGN_TOP);
	
	m_GridCurrencyPairs->CreateGrid(1, 4);
	m_GridCurrencyPairs->SetColLabelValue(C_TaxCurrencySettings::S_TaxCurrencyTableRow::E_CollType::Base, "Base Cur.");
	m_GridCurrencyPairs->SetColLabelValue(C_TaxCurrencySettings::S_TaxCurrencyTableRow::E_CollType::Quote, "Quote Cur.");
	m_GridCurrencyPairs->SetColLabelValue(C_TaxCurrencySettings::S_TaxCurrencyTableRow::E_CollType::Year, "Year");
	m_GridCurrencyPairs->SetColLabelValue(C_TaxCurrencySettings::S_TaxCurrencyTableRow::E_CollType::Rate, "Rate");
	m_GridCurrencyPairs->SetColFormatNumber(C_TaxCurrencySettings::S_TaxCurrencyTableRow::E_CollType::Year);
	m_GridCurrencyPairs->SetColFormatFloat(C_TaxCurrencySettings::S_TaxCurrencyTableRow::E_CollType::Rate);
	m_GridCurrencyPairs->ForceRefresh();
	wxSize gridMinSize = m_GridCurrencyPairs->GetBestSize();
	gridMinSize.x += wxSystemSettings::GetMetric(wxSYS_HSCROLL_Y);
	gridMinSize.y += m_GridCurrencyPairs->GetDefaultRowSize() * 9;	//reserve space for 10 rows in overall
	m_GridCurrencyPairs->SetMinSize(gridMinSize);
    currencyPairsGridSizer->Add(m_GridCurrencyPairs, PROPORTION, wxEXPAND | wxALL);

	topSizer->Add(currencyPairsGridSizer, PROPORTION, wxEXPAND | wxALL, BORDER_SMALL);

	wxBoxSizer * buttonsSizer = new wxBoxSizer(wxHORIZONTAL);
	wxButton * const applyButton = new wxButton(this, ID_Apply, wxT("Sort, Save and Close"));
	applyButton->Bind(wxEVT_BUTTON, &C_TaxCurrencySettingsDialog::OnApply, this);
	buttonsSizer->Add(applyButton, PROPORTION, wxEXPAND | wxALL, BORDER_SMALL);
	wxButton * const cancelButton = new wxButton(this, wxID_CANCEL, wxT("Cancel"));
	buttonsSizer->Add(cancelButton, PROPORTION, wxEXPAND | wxALL, BORDER_SMALL);

	topSizer->Add(buttonsSizer, PROPORTION, wxALIGN_RIGHT | wxTOP | wxBOTTOM, BORDER_BIG);

	ReadData();

	SetSizerAndFit(topSizer);

	Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(C_TaxCurrencySettingsDialog::OnKeyDown), nullptr, this);
	Connect(wxEVT_GRID_CELL_CHANGED, wxGridEventHandler(C_TaxCurrencySettingsDialog::OnGridChanged), nullptr, this);
}

void C_TaxCurrencySettingsDialog::OnKeyDown(wxKeyEvent & event)
{
	assert(m_GridCurrencyPairs != nullptr);

	switch (event.GetKeyCode())
	{
		case WXK_DELETE:
		case WXK_BACK:
		{
			for (wxGridBlockCoords const & block : m_GridCurrencyPairs->GetSelectedBlocks())
			{
				for (int row = block.GetTopRow(); row <= block.GetBottomRow(); row++)
				{
					for (int col = block.GetLeftCol(); col <= block.GetRightCol(); col++)
					{
						m_GridCurrencyPairs->SetCellValue(row, col, "");
					}
				}
			}
		}
		break;

		default:
		{
			event.Skip();
		}
	}
}

void C_TaxCurrencySettingsDialog::OnApply(wxCommandEvent & /*event*/)
{
	RemoveInvalidData();
	SortData();
	RemoveDuplicates();
	WriteData();

	wxDialog::SetAffirmativeId(wxID_OK);
    wxDialog::AcceptAndClose();
}

void C_TaxCurrencySettingsDialog::OnGridChanged(wxGridEvent & /*event*/)
{
	assert(m_GridCurrencyPairs != nullptr);

	int const rowCount (m_GridCurrencyPairs->GetNumberRows());
	if (rowCount > 0)
	{
		int lastRowIndex (rowCount - 1);
		bool lastRowEmpty (true);
		for (int colIndex = 0; colIndex < m_GridCurrencyPairs->GetNumberCols(); ++colIndex)
		{
			wxString const & colValue = m_GridCurrencyPairs->GetCellValue(lastRowIndex, colIndex);
			if (!colValue.IsEmpty())
			{
				lastRowEmpty = false;
				break;
			}
		}

		if (!lastRowEmpty)
		{
			m_GridCurrencyPairs->AppendRows();
		}
	}
}

void C_TaxCurrencySettingsDialog::ReadData()
{
	m_TaxCurrencyCtrl->SetValue(m_TaxCurrencySettings.GetCurrencyType());

	if (m_GridCurrencyPairs->AppendRows(m_TaxCurrencySettings.GetTable().size()))
	{
		for (unsigned rowCounter = 0; rowCounter < m_TaxCurrencySettings.GetTable().size(); ++rowCounter)
		{
			C_TaxCurrencySettings::S_TaxCurrencyTableRow const & row = m_TaxCurrencySettings.GetTable().at(rowCounter);

			SetGridCurrencyPairsRow(rowCounter, row);
		}
	}
}

void C_TaxCurrencySettingsDialog::RemoveInvalidData()
{
	for (unsigned rowIndex = 0; rowIndex < m_GridCurrencyPairs->GetNumberRows();)
	{
		C_TaxCurrencySettings::S_TaxCurrencyTableRow row;
		if (GetGridCurrencyPairsRow(rowIndex, row))
		{
			assert (row.IsValid());
			rowIndex++;
		}
		else
		{
			m_GridCurrencyPairs->DeleteRows(rowIndex);
		}
	}
}

void C_TaxCurrencySettingsDialog::SortData()
{
	for (unsigned prevRowIndex = 0; prevRowIndex < m_GridCurrencyPairs->GetNumberRows(); ++prevRowIndex)
	{
		C_TaxCurrencySettings::S_TaxCurrencyTableRow prevRow;
		GetGridCurrencyPairsRow(prevRowIndex, prevRow);

		for (unsigned rowIndex = prevRowIndex + 1; rowIndex < m_GridCurrencyPairs->GetNumberRows(); ++rowIndex)
		{
			C_TaxCurrencySettings::S_TaxCurrencyTableRow row;
			if (GetGridCurrencyPairsRow(rowIndex, row))
			{
				if (row < prevRow)
				{
					SetGridCurrencyPairsRow(prevRowIndex, row);
					SetGridCurrencyPairsRow(rowIndex, prevRow);
				}
			}
		}
	}
}

void C_TaxCurrencySettingsDialog::RemoveDuplicates()
{
	unsigned rowIndex = 0;
	unsigned nextRowIndex = rowIndex + 1;
	for (; nextRowIndex < m_GridCurrencyPairs->GetNumberRows();)
	{
		C_TaxCurrencySettings::S_TaxCurrencyTableRow row;
		GetGridCurrencyPairsRow(rowIndex, row);

		C_TaxCurrencySettings::S_TaxCurrencyTableRow nextRow;
		GetGridCurrencyPairsRow(nextRowIndex, nextRow);

		if (!(row < nextRow))
		{
			m_GridCurrencyPairs->DeleteRows(nextRowIndex);
		}
		else
		{
			rowIndex++;
			nextRowIndex++;
		}
	}
}

void C_TaxCurrencySettingsDialog::WriteData()
{
	m_TaxCurrencySettings.Clear();

	std::string currencyType (m_TaxCurrencyCtrl->GetValue());
	m_TaxCurrencySettings.SetCurrencyType(currencyType);
	
	for (unsigned rowIndex = 0; rowIndex < m_GridCurrencyPairs->GetNumberRows(); ++rowIndex)
	{
		C_TaxCurrencySettings::S_TaxCurrencyTableRow row;
		if (GetGridCurrencyPairsRow(rowIndex, row))
		{
			m_TaxCurrencySettings.AddTableRow(row);
		}
	}
}

bool C_TaxCurrencySettingsDialog::GetGridCurrencyPairsRow(unsigned const rowIndex, C_TaxCurrencySettings::S_TaxCurrencyTableRow & row)
{
	assert (rowIndex < m_GridCurrencyPairs->GetNumberRows());

	bool error = false;

	for (unsigned colIndex = 0; colIndex < m_GridCurrencyPairs->GetNumberCols(); ++colIndex)
	{
		assert(colIndex < C_TaxCurrencySettings::S_TaxCurrencyTableRow::E_CollType::_Count);
		C_TaxCurrencySettings::S_TaxCurrencyTableRow::E_CollType const collType (static_cast<C_TaxCurrencySettings::S_TaxCurrencyTableRow::E_CollType> (colIndex));

		wxString const & colValue = m_GridCurrencyPairs->GetCellValue(rowIndex, colIndex);
		if (!colValue.IsEmpty())
		{
			switch (collType)
			{
				case C_TaxCurrencySettings::S_TaxCurrencyTableRow::E_CollType::Base:
				{
					row.m_Base = colValue;
				}
				break;

				case C_TaxCurrencySettings::S_TaxCurrencyTableRow::E_CollType::Quote:
				{
					row.m_Quote = colValue;
				}
				break;

				case C_TaxCurrencySettings::S_TaxCurrencyTableRow::E_CollType::Year:
				{
					row.m_Year = wxAtoi(colValue);
					if (row.m_Year == 0)
					{
						error = true;
						break;
					}
				}
				break;

				case C_TaxCurrencySettings::S_TaxCurrencyTableRow::E_CollType::Rate:
				{
					if (!StringToDecimal(colValue.c_str(), row.m_Rate) || row.m_Rate.IsZero())
					{
						error = true;
						break;
					}
				}
				break;

				default:
					assert(!"Unknown col type!");
			}
		}
		else
		{
			error = true;
			break;
		}
	}

	return !error;
}

void C_TaxCurrencySettingsDialog::SetGridCurrencyPairsRow(unsigned const rowIndex, C_TaxCurrencySettings::S_TaxCurrencyTableRow const & row)
{
	assert (m_GridCurrencyPairs != nullptr);
	assert(rowIndex < m_GridCurrencyPairs->GetNumberRows());

	m_GridCurrencyPairs->SetCellValue(rowIndex, C_TaxCurrencySettings::S_TaxCurrencyTableRow::E_CollType::Base, row.m_Base);
	m_GridCurrencyPairs->SetCellValue(rowIndex, C_TaxCurrencySettings::S_TaxCurrencyTableRow::E_CollType::Quote, row.m_Quote);
	m_GridCurrencyPairs->SetCellValue(rowIndex, C_TaxCurrencySettings::S_TaxCurrencyTableRow::E_CollType::Year, wxString::Format(wxT("%u"), row.m_Year));
	m_GridCurrencyPairs->SetCellValue(rowIndex, C_TaxCurrencySettings::S_TaxCurrencyTableRow::E_CollType::Rate, row.m_Rate.GetAsString());
}