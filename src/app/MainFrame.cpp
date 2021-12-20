//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
//*********************************************************************************************************************

#include "MainFrame.h"
#include "Ids.h"
#include <trade_book/TradeBook.h>
#include <trade_book/TradeItemMarket.h>
#include <tax/TaxFifo.h>
#include <input/FileReader.h>
#include <types/Decimal.h>

#include <wx/grid.h>

#include <iomanip>      // std::put_time


C_MainFrame::C_MainFrame()
    : wxFrame(NULL, wxID_ANY, "Bitstamp Tax", wxDefaultPosition, wxSize(1024, 768))
	, m_ListBoxFile(nullptr)
	, m_GridPairs(nullptr)
	, m_GridFees(nullptr)
    , m_GridTaxes(nullptr)
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_FileOpen, "&Open...\tCtrl-O", "Open File...");
    menuFile->Append(wxID_EXIT);
    
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);

    CreateStatusBar();
    SetStatusText("Ready");

    wxBoxSizer * topSizer = new wxBoxSizer(wxVERTICAL);
    
    //First row - Input data
	wxBoxSizer * fileSizer = new wxBoxSizer(wxVERTICAL);
	wxStaticText * const captionFile (new wxStaticText(this, wxID_ANY, wxT("File data:")));
	wxFont captionFont (captionFile->GetFont());
//	captionFont.SetPointSize(10);
	captionFont.SetWeight(wxFONTWEIGHT_BOLD);
	captionFile->SetFont(captionFont);
	fileSizer->Add(captionFile, 0, wxALIGN_LEFT);
    m_ListBoxFile = new wxListBox(this, ID_ListBoxFile);
    fileSizer->Add(m_ListBoxFile, 1, wxEXPAND | wxALL);
	topSizer->Add(fileSizer, 1, wxEXPAND | wxALL);

	//Second row - Buy-sell pairs
	wxBoxSizer * pairsGridSizer = new wxBoxSizer(wxVERTICAL);
	wxStaticText * const captionPairs (new wxStaticText(this, wxID_ANY, wxT("Trade pairs:")));
	captionPairs->SetFont(captionFont);
	pairsGridSizer->Add(captionPairs, 0, wxALIGN_LEFT);
	m_GridPairs = new wxGrid(this, ID_PairsGrid, wxPoint(0, 0), FromDIP(wxSize(800, 70)));	//The size definition fixes a weird row overflow out of sizer border
	m_GridPairs->EnableEditing(false);
	m_GridPairs->SetDefaultCellAlignment(wxALIGN_RIGHT, wxALIGN_TOP);
	
	m_GridPairs->CreateGrid(0, 6);
	m_GridPairs->SetColLabelValue(0, "Sell Datetime");
	m_GridPairs->SetColLabelValue(1, "Sell Price");
	m_GridPairs->SetColLabelValue(2, "Amount");
	m_GridPairs->SetColLabelValue(3, "Buy Datetime");
	m_GridPairs->SetColLabelValue(4, "Buy Price");
	m_GridPairs->SetColLabelValue(5, "P/L");
	pairsGridSizer->Add(m_GridPairs, 1, wxEXPAND | wxALL);
	topSizer->Add(pairsGridSizer, 1, wxEXPAND | wxALL);

    //Third row - Fee
	wxBoxSizer * feesGridSizer = new wxBoxSizer(wxVERTICAL);
	wxStaticText * const captionFees (new wxStaticText(this, wxID_ANY, wxT("Fees:")));
	captionFees->SetFont(captionFont);
	feesGridSizer->Add(captionFees, 0, wxALIGN_LEFT);
    m_GridFees = new wxGrid(this, ID_PairsGrid, wxPoint(0, 0), FromDIP(wxSize(800, 70)));
	m_GridFees->EnableEditing(false);
	m_GridFees->SetDefaultCellAlignment(wxALIGN_RIGHT, wxALIGN_TOP);
	m_GridFees->CreateGrid(0, 2);
	m_GridFees->SetColLabelValue(0, "Datetime");
	m_GridFees->SetColLabelValue(1, "Fee");
	feesGridSizer->Add(m_GridFees, 1, wxEXPAND | wxALL);
	topSizer->Add(feesGridSizer, 1, wxEXPAND | wxALL);

	//Fourth row - Tax
	wxBoxSizer * taxesGridSizer = new wxBoxSizer(wxVERTICAL);
	wxStaticText * const captionTax (new wxStaticText(this, wxID_ANY, wxT("Tax:")));
	captionTax->SetFont(captionFont);
	taxesGridSizer->Add(captionTax, 0, wxALIGN_LEFT);
    m_GridTaxes = new wxGrid(this, ID_PairsGrid, wxPoint(0, 0), FromDIP(wxSize(800, 70)));
	m_GridTaxes->EnableEditing(false);
	m_GridTaxes->SetDefaultCellAlignment(wxALIGN_RIGHT, wxALIGN_TOP);
	
	m_GridTaxes->CreateGrid(0, 4);
	m_GridTaxes->SetColLabelValue(0, "Year");
	m_GridTaxes->SetColLabelValue(1, "Expenditures");
	m_GridTaxes->SetColLabelValue(2, "Receipts");
	m_GridTaxes->SetColLabelValue(3, "P/L");
    taxesGridSizer->Add(m_GridTaxes, 1, wxEXPAND | wxALL);

	topSizer->Add(taxesGridSizer, 1, wxEXPAND | wxALL);

	this->SetSizer(topSizer);

    Bind(wxEVT_MENU, &C_MainFrame::OnOpenFile, this, ID_FileOpen);
    Bind(wxEVT_MENU, &C_MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &C_MainFrame::OnExit, this, wxID_EXIT);
}

void C_MainFrame::OnOpenFile(wxCommandEvent & event)
{
    wxFileDialog openFileDialog(this,
        _("Open data file"),
        "",
        "",
        "Data files (*.csv)|*.csv",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    switch (openFileDialog.ShowModal())
    {
        case wxID_OK:
        {
			m_FileReader = nullptr;
			m_TradeBook = nullptr;
			m_TaxFifo = nullptr;
			OnDataChanged();

            m_FileReader = std::make_unique<C_FileReader>(openFileDialog.GetPath().ToStdString());
		    if ((m_FileReader!= nullptr) && (m_FileReader->Read()))
            {
                m_TradeBook = std::make_unique<C_TradeBook>();
                m_TradeBook->SetTradeItems(std::move(m_FileReader->MoveTradeItems()));

				if ((m_TradeBook != nullptr) && !m_TradeBook->IsEmpty())
				{
					T_CurrencyType const taxCurrency(m_TradeBook->AssessUserCurrency());
					m_TaxFifo = std::make_unique<C_TaxFifo>(taxCurrency);
					if (!m_TaxFifo->Process(*m_TradeBook))
					{
						std::string errorMsg;
						if (!m_TaxFifo->GetErrorMsg().empty())
						{
							errorMsg = " Error: ";
							errorMsg += m_TaxFifo->GetErrorMsg();
						}
						wxLogError("Tax computation failed, probably incomplete data has been passed.%s", errorMsg);
						m_TaxFifo = nullptr;
					}
				}
            }
            else
            {
				std::string errorMsg;
				if (!m_FileReader->GetErrorMsg().empty())
				{
					errorMsg = " Error: ";
					errorMsg += m_FileReader->GetErrorMsg();
				}
                wxLogError("Cannot read file '%s'.%s", openFileDialog.GetPath(), errorMsg);
				m_FileReader = nullptr;
            }

			OnDataChanged();
        }
        break;

        case wxID_CANCEL:
        {
        }
        break;

        default:
            assert(0);
    }
}

void C_MainFrame::OnExit(wxCommandEvent & event)
{
    Close(true);
}

void C_MainFrame::OnAbout(wxCommandEvent & event)
{
    wxMessageBox("This is a simple tool to deal with a tax computation after a crypto trading fun. Use it at you own risk.",
                 "About BitstampTax", wxOK | wxICON_INFORMATION);
}

void C_MainFrame::OnDataChanged()
{
	UpdateListBoxFile();
	UpdateGridPairs();
	UpdateGridFees();
	UpdateGridTaxes();
}

void C_MainFrame::UpdateListBoxFile()
{
	assert(m_ListBoxFile != nullptr);
	m_ListBoxFile->Clear();

	if (m_FileReader != nullptr)
	{
		wxListBox & listBox = *m_ListBoxFile;
		m_FileReader->EnumerateLines([&listBox](std::string const & line) -> void
		{
			listBox.AppendString(line);
		});
	}
}

void C_MainFrame::UpdateGridPairs()
{
	assert(m_GridPairs != nullptr);

	m_GridPairs->ClearGrid();

	if (m_GridPairs->GetNumberRows() > 0)
	{
		m_GridPairs->DeleteRows(0, m_GridPairs->GetNumberRows());
	}

	if (m_TaxFifo != nullptr)
	{
		m_GridPairs->AppendRows(m_TaxFifo->GetPairsCount());
		int rowCounter(0);
		m_TaxFifo->EnumerateTradePairs([this, &rowCounter](C_TradePair const & tradePair) -> void
		{
			m_GridPairs->SetCellValue(rowCounter, 0, GetDateTimeString(tradePair.GetSellTime()));
			m_GridPairs->SetCellValue(rowCounter, 1, tradePair.GetSellPrice().GetAsString());
			
			m_GridPairs->SetCellValue(rowCounter, 2, tradePair.GetAmount().GetAsString());
			
			m_GridPairs->SetCellValue(rowCounter, 3, GetDateTimeString(tradePair.GetBuyTime()));
			m_GridPairs->SetCellValue(rowCounter, 4, tradePair.GetBuyPrice().GetAsString());

			C_CurrencyValue const expenditure(tradePair.GetBuyPrice() * tradePair.GetAmount().GetValue());
			C_CurrencyValue const receipt(tradePair.GetSellPrice() * tradePair.GetAmount().GetValue());
			C_CurrencyValue const profitOrLoss(receipt - expenditure);
			m_GridPairs->SetCellValue(rowCounter, 5, profitOrLoss.GetAsString());

			rowCounter++;
		});

		m_GridPairs->AutoSizeColumns();
	}
}

void C_MainFrame::UpdateGridFees()
{
	assert(m_GridFees != nullptr);
	
	m_GridFees->ClearGrid();

	if (m_GridFees->GetNumberRows() > 0)
	{
		m_GridFees->DeleteRows(0, m_GridFees->GetNumberRows());
	}

	if (m_TradeBook != nullptr)
	{
		m_GridFees->AppendRows(m_TradeBook->GetData().size());
		int rowCounter(0);

		for (T_TradeItemUniquePtr const & item : m_TradeBook->GetData())
		{
			C_TradeItemMarket const * const itemMarket (dynamic_cast<C_TradeItemMarket const *>(item.get()));
			if (itemMarket != nullptr)
			{
				C_CurrencyValue fee;
				if (itemMarket->IsFeeValid())
				{
					m_GridFees->SetCellValue(rowCounter, 0, GetDateTimeString(itemMarket->GetTime()));
					m_GridFees->SetCellValue(rowCounter, 1, itemMarket->GetFee().GetAsString());

					rowCounter++;
				}
			}
		}

		if (rowCounter < m_GridFees->GetNumberRows())
		{
			m_GridFees->DeleteRows(rowCounter, m_GridFees->GetNumberRows() - rowCounter);
		}
	}
}

void C_MainFrame::UpdateGridTaxes()
{
	assert(m_GridTaxes != nullptr);

	m_GridTaxes->ClearGrid();

	if (m_GridTaxes->GetNumberRows() > 0)
	{
		m_GridTaxes->DeleteRows(0, m_GridTaxes->GetNumberRows());
	}

	if (m_TaxFifo != nullptr)
	{
		m_GridTaxes->AppendRows(m_TaxFifo->GetTaxCount());
		int rowCounter(0);
		m_TaxFifo->EnumerateTaxes([this, &rowCounter](C_Tax const & tax) -> void
		{
			m_GridTaxes->SetCellValue(rowCounter, 0, wxString::Format("%d", tax.GetYear()));
			m_GridTaxes->SetCellValue(rowCounter, 1, tax.GetExpenditures().GetAsString());
			m_GridTaxes->SetCellValue(rowCounter, 2, tax.GetReceipts().GetAsString());
			m_GridTaxes->SetCellValue(rowCounter, 3, tax.GetProfit().GetAsString());

			rowCounter++;
		});

		m_GridTaxes->AutoSizeColumns();
	}
}

wxString C_MainFrame::GetDateTimeString(time_t const t)
{
	struct tm const * const tmTime(localtime(&t));

	static size_t const STR_TIME_LEN(80);
	char strTime [STR_TIME_LEN];
	if (strftime(strTime, STR_TIME_LEN, "%d-%b-%Y %H:%M", tmTime) == 0)
	{
		strTime[0] = '\0';
	}

	return wxString(strTime);
}