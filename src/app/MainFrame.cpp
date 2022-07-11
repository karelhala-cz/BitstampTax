//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021-2022 Karel Hala
// License:	  MIT
//*********************************************************************************************************************

#include "MainFrame.h"
#include "Ids.h"
#include "AboutDialog.h"
#include "TaxCurrencySettings.h"
#include "TaxCurrencySettingsDialog.h"
#include <trade_book/TradeBook.h>
#include <trade_book/TradeItemMarket.h>
#include <tax/TaxFifo.h>
#include <input/FileReader.h>
#include <types/Decimal.h>
#include <utils/TimeUtils.h>

#include <wx/grid.h>
#include <wx/stdpaths.h>

#include <iomanip>      // std::put_time

//static
char const * const C_MainFrame::FILE_NAME_TAX_CURRENCY_SETTINGS = "TaxCurrencySettings.txt";

//static
bool C_MainFrame::EnsureDirExists(std::filesystem::path const & dir)
{
	std::error_code ec;
	bool const created (std::filesystem::create_directory(dir, ec));
	
	return created || !ec;	//Created or already exists
}

//static
std::string C_MainFrame::GetTaxCurrencySettingsDefaultFilePath()
{
	std::string filename (wxStandardPaths::Get().GetUserDataDir());
	filename += "\\";
	filename += FILE_NAME_TAX_CURRENCY_SETTINGS;

	return filename;
}

C_MainFrame::C_MainFrame()
    : wxFrame(NULL, wxID_ANY, "Bitstamp Tax", wxDefaultPosition, wxSize(1024, 768))
	, m_ListBoxFile(nullptr)
	, m_GridPairs(nullptr)
	, m_GridFees(nullptr)
    , m_GridTaxes(nullptr)
	, m_TaxCurrencySettings(std::make_unique<C_TaxCurrencySettings>())
{
	wxMenu * const menuFile = new wxMenu;
    menuFile->Append(ID_FileOpen, "&Open...\tCtrl-O", "Open File...");
	menuFile->Append(ID_TaxCurrencySettings, "Tax Currency Settings...");
    menuFile->Append(wxID_EXIT);
    
    wxMenu * const menuHelp = new wxMenu;
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
	m_GridPairs = new wxGrid(this, ID_PairsGrid, wxPoint(0, 0), FromDIP(wxSize(1200, 70)));	//The size definition fixes a weird row overflow out of sizer border
	m_GridPairs->EnableEditing(false);
	m_GridPairs->SetDefaultCellAlignment(wxALIGN_RIGHT, wxALIGN_TOP);
	
	m_GridPairs->CreateGrid(0, static_cast<int>(E_GridPairsCollType::_Count));
	m_GridPairs->SetColLabelValue(static_cast<int>(E_GridPairsCollType::SellDatetime), "Sell Datetime");
	m_GridPairs->SetColLabelValue(static_cast<int>(E_GridPairsCollType::SellPrice), "Sell Price");
	m_GridPairs->SetColLabelValue(static_cast<int>(E_GridPairsCollType::Amount), "Amount");
	m_GridPairs->SetColLabelValue(static_cast<int>(E_GridPairsCollType::BuyDatetime), "Buy Datetime");
	m_GridPairs->SetColLabelValue(static_cast<int>(E_GridPairsCollType::BuyPrice), "Buy Price");
	m_GridPairs->SetColLabelValue(static_cast<int>(E_GridPairsCollType::Proceeds), "Proceeds");
	m_GridPairs->SetColLabelValue(static_cast<int>(E_GridPairsCollType::Cost), "Cost");
	m_GridPairs->SetColLabelValue(static_cast<int>(E_GridPairsCollType::PnL), "P/L");
	m_GridPairs->SetColLabelValue(static_cast<int>(E_GridPairsCollType::SellCurrencyRate), "Sell Cur. Rate");
	m_GridPairs->SetColLabelValue(static_cast<int>(E_GridPairsCollType::ProceedsInTaxCurrency), "Proceeds [Tax cur.]");
	m_GridPairs->SetColLabelValue(static_cast<int>(E_GridPairsCollType::BuyCurrencyRate), "Buy Cur. Rate");
	m_GridPairs->SetColLabelValue(static_cast<int>(E_GridPairsCollType::CostInTaxCurrency), "Cost [Tax cur.]");
	m_GridPairs->SetColLabelValue(static_cast<int>(E_GridPairsCollType::PnLInTaxCurrency), "P/L [Tax cur.]");
	m_GridPairs->Fit();
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
	m_GridFees->CreateGrid(0, static_cast<int>(E_GridFeesCollType::_Count));
	m_GridFees->SetColLabelValue(static_cast<int>(E_GridFeesCollType::Datetime), "Datetime");
	m_GridFees->SetColLabelValue(static_cast<int>(E_GridFeesCollType::Fee), "Fee");
	m_GridFees->SetColLabelValue(static_cast<int>(E_GridFeesCollType::FeeCurrencyRate), "Fee Cur. Rate");
	m_GridFees->SetColLabelValue(static_cast<int>(E_GridFeesCollType::FeeInTaxCurrency), "Fee [Tax cur.]");
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
	
	m_GridTaxes->CreateGrid(0, static_cast<int>(E_GridTaxesCollType::_Count));
	m_GridTaxes->SetColLabelValue(static_cast<int>(E_GridTaxesCollType::Year), "Year");
	m_GridTaxes->SetColLabelValue(static_cast<int>(E_GridTaxesCollType::Expenditures), "Expenditures");
	m_GridTaxes->SetColLabelValue(static_cast<int>(E_GridTaxesCollType::Receipts), "Receipts");
	m_GridTaxes->SetColLabelValue(static_cast<int>(E_GridTaxesCollType::PnL), "P/L");
	m_GridTaxes->SetColLabelValue(static_cast<int>(E_GridTaxesCollType::ExpendituresInTaxCurrency), "Expenditures [Tax cur.]");
	m_GridTaxes->SetColLabelValue(static_cast<int>(E_GridTaxesCollType::ReceiptsInTaxCurrency), "Receipts [Tax cur.]");
	m_GridTaxes->SetColLabelValue(static_cast<int>(E_GridTaxesCollType::PnLInTaxCurrency), "P/L [Tax cur.]");
    taxesGridSizer->Add(m_GridTaxes, 1, wxEXPAND | wxALL);

	topSizer->Add(taxesGridSizer, 1, wxEXPAND | wxALL);

	this->SetSizer(topSizer);

    Bind(wxEVT_MENU, &C_MainFrame::OnOpenFile, this, ID_FileOpen);
	Bind(wxEVT_MENU, &C_MainFrame::OnTaxCurrencySettings, this, ID_TaxCurrencySettings);
    Bind(wxEVT_MENU, &C_MainFrame::OnExit, this, wxID_EXIT);
	Bind(wxEVT_MENU, &C_MainFrame::OnAbout, this, wxID_ABOUT);
	Bind(wxEVT_LISTBOX,  &C_MainFrame::OnListBoxFile, this, ID_ListBoxFile);

	LoadTaxCurrencySettings();
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
					m_TaxFifo = std::make_unique<C_TaxFifo>(taxCurrency, *m_TaxCurrencySettings);
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

void C_MainFrame::OnTaxCurrencySettings(wxCommandEvent & event)
{
	C_TaxCurrencySettingsDialog taxCurrencySettingsDialog(*m_TaxCurrencySettings);
	if (taxCurrencySettingsDialog.ShowModal() == wxID_OK)
	{
		SaveTaxCurrencySettings();
	}
}

void C_MainFrame::OnExit(wxCommandEvent & event)
{
    Close(true);
}

void C_MainFrame::OnAbout(wxCommandEvent & event)
{
	C_AboutDialog aboutDialog;
	if (aboutDialog.ShowModal() == wxID_OK)
	{

	}
}

void C_MainFrame::OnListBoxFile(wxCommandEvent & event)
{
	int const selectedLineNumber (event.GetInt());
	assert (selectedLineNumber >= 0);
	OnSelectedFileLine(static_cast<size_t>(selectedLineNumber));
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
			m_GridPairs->SetCellValue(rowCounter, static_cast<int>(E_GridPairsCollType::SellDatetime), GetDateTimeString(tradePair.GetSellTime()));
			m_GridPairs->SetCellValue(rowCounter, static_cast<int>(E_GridPairsCollType::SellPrice), tradePair.GetSellPrice().GetAsString());
			
			m_GridPairs->SetCellValue(rowCounter, static_cast<int>(E_GridPairsCollType::Amount), tradePair.GetAmount().GetAsString());
			
			m_GridPairs->SetCellValue(rowCounter, static_cast<int>(E_GridPairsCollType::BuyDatetime), GetDateTimeString(tradePair.GetBuyTime()));
			m_GridPairs->SetCellValue(rowCounter, static_cast<int>(E_GridPairsCollType::BuyPrice), tradePair.GetBuyPrice().GetAsString());
			
			C_CurrencyValue const proceeds(tradePair.GetSellPrice() * tradePair.GetAmount().GetValue());
			m_GridPairs->SetCellValue(rowCounter, static_cast<int>(E_GridPairsCollType::Proceeds), proceeds.GetAsString());

			C_CurrencyValue const cost(tradePair.GetBuyPrice() * tradePair.GetAmount().GetValue());
			m_GridPairs->SetCellValue(rowCounter, static_cast<int>(E_GridPairsCollType::Cost), cost.GetAsString());

			C_CurrencyValue const profitOrLoss(proceeds - cost);
			m_GridPairs->SetCellValue(rowCounter, static_cast<int>(E_GridPairsCollType::PnL), profitOrLoss.GetAsString());

			C_Decimal sellCurrencyRate;
			C_CurrencyValue const proceedsInTaxCurrency(ConvertToTaxCurrency(proceeds, tradePair.GetSellTime(), sellCurrencyRate));
			m_GridPairs->SetCellValue(rowCounter, static_cast<int>(E_GridPairsCollType::SellCurrencyRate), sellCurrencyRate.GetAsString());
			m_GridPairs->SetCellValue(rowCounter, static_cast<int>(E_GridPairsCollType::ProceedsInTaxCurrency), proceedsInTaxCurrency.GetAsString());

			C_Decimal buyCurrencyRate;
			C_CurrencyValue const costInTaxCurrency(ConvertToTaxCurrency(cost, tradePair.GetBuyTime(), buyCurrencyRate));
			m_GridPairs->SetCellValue(rowCounter, static_cast<int>(E_GridPairsCollType::BuyCurrencyRate), buyCurrencyRate.GetAsString());
			m_GridPairs->SetCellValue(rowCounter, static_cast<int>(E_GridPairsCollType::CostInTaxCurrency), costInTaxCurrency.GetAsString());

			C_CurrencyValue const profitOrLossInTaxCurrency(proceedsInTaxCurrency - costInTaxCurrency);
			m_GridPairs->SetCellValue(rowCounter, static_cast<int>(E_GridPairsCollType::PnLInTaxCurrency), profitOrLossInTaxCurrency.GetAsString());

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
					m_GridFees->SetCellValue(rowCounter, static_cast<int>(E_GridFeesCollType::Datetime), GetDateTimeString(itemMarket->GetTime()));
					m_GridFees->SetCellValue(rowCounter, static_cast<int>(E_GridFeesCollType::Fee), itemMarket->GetFee().GetAsString());

					C_Decimal currencyRate;
					C_CurrencyValue const feeInTaxCurrency(ConvertToTaxCurrency(itemMarket->GetFee(), itemMarket->GetTime(), currencyRate));
					m_GridFees->SetCellValue(rowCounter, static_cast<int>(E_GridFeesCollType::FeeCurrencyRate), currencyRate.GetAsString());
					m_GridFees->SetCellValue(rowCounter, static_cast<int>(E_GridFeesCollType::FeeInTaxCurrency), feeInTaxCurrency.GetAsString());

					rowCounter++;
				}
			}
		}

		if (rowCounter < m_GridFees->GetNumberRows())
		{
			m_GridFees->DeleteRows(rowCounter, m_GridFees->GetNumberRows() - rowCounter);
		}

		m_GridFees->AutoSizeColumns();
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
			m_GridTaxes->SetCellValue(rowCounter, static_cast<int>(E_GridTaxesCollType::Year), wxString::Format("%d", tax.GetYear()));
			
			m_GridTaxes->SetCellValue(rowCounter, static_cast<int>(E_GridTaxesCollType::Expenditures), tax.GetExpenditures().GetAsString());
			m_GridTaxes->SetCellValue(rowCounter, static_cast<int>(E_GridTaxesCollType::Receipts), tax.GetReceipts().GetAsString());
			m_GridTaxes->SetCellValue(rowCounter, static_cast<int>(E_GridTaxesCollType::PnL), tax.GetProfit().GetAsString());

			m_GridTaxes->SetCellValue(rowCounter, static_cast<int>(E_GridTaxesCollType::ExpendituresInTaxCurrency), tax.GetExpendituresInTaxCurrency().GetAsString());
			m_GridTaxes->SetCellValue(rowCounter, static_cast<int>(E_GridTaxesCollType::ReceiptsInTaxCurrency), tax.GetReceiptsInTaxCurrency().GetAsString());
			m_GridTaxes->SetCellValue(rowCounter, static_cast<int>(E_GridTaxesCollType::PnLInTaxCurrency), tax.GetProfitInTaxCurrency().GetAsString());

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

void C_MainFrame::OnSelectedFileLine(size_t const inputFileLine)
{
	UpdateGridPairsSelection(inputFileLine);
	UpdateGridFeesSelection(inputFileLine);
}

void C_MainFrame::UpdateGridPairsSelection(size_t const selectedFileLine)
{
	m_GridPairs->ClearSelection();

	enum class E_SellectedLineType { UNDEFINED, BUY, SELL};
	E_SellectedLineType selectedLineType(E_SellectedLineType::UNDEFINED);
	size_t firstRowIndex(m_TaxFifo->GetPairsCount());
	size_t lastRowIndex(m_TaxFifo->GetPairsCount());

	if (m_TaxFifo != nullptr)
	{
		for (size_t i = 0; i < m_TaxFifo->GetPairsCount(); ++i)
		{
			C_TradePair const & tradePair(m_TaxFifo->GetTradePair(i));
			if ((tradePair.GetBuyInputFileLine() == selectedFileLine) || (tradePair.GetSellInputFileLine() == selectedFileLine))
			{
				if (selectedLineType == E_SellectedLineType::UNDEFINED)
				{
					selectedLineType = (tradePair.GetBuyInputFileLine() == selectedFileLine) ? E_SellectedLineType::BUY : E_SellectedLineType::SELL;
					firstRowIndex = i;
					lastRowIndex = i;
				}
				else
				{
					assert(((tradePair.GetBuyInputFileLine() == selectedFileLine) && (selectedLineType == E_SellectedLineType::BUY))||
						((tradePair.GetSellInputFileLine() == selectedFileLine) && (selectedLineType == E_SellectedLineType::SELL)));
					assert(lastRowIndex <= i);
					lastRowIndex = i;
				}
			}
			else
			{
				if (selectedLineType != E_SellectedLineType::UNDEFINED)
				{
					break;
				}
			}
		}
	}

	if (selectedLineType != E_SellectedLineType::UNDEFINED)
	{
		static int const FIRST_COLL_SELL_SELECTION(0);
		static int const FIRST_COLL_BUY_SELECTION(2);
		static int const COLL_SELECTION_COUNT(2);
		int const firstCollIndex((selectedLineType == E_SellectedLineType::SELL) ? FIRST_COLL_SELL_SELECTION : FIRST_COLL_BUY_SELECTION);
		int const lastCollIndex(firstCollIndex + COLL_SELECTION_COUNT);
		wxGridCellCoords const topLeft(firstRowIndex, firstCollIndex);
		wxGridCellCoords const bottomRight(lastRowIndex, lastCollIndex);
		m_GridPairs->SelectBlock(topLeft, bottomRight);
		m_GridPairs->MakeCellVisible(topLeft);
		m_GridPairs->MakeCellVisible(bottomRight);
	}
}

void C_MainFrame::UpdateGridFeesSelection(size_t const selectedFileLine)
{
	m_GridFees->ClearSelection();

	if (m_TradeBook != nullptr)
	{
		int rowIndex(m_GridFees->GetNumberRows());

		size_t gridFeeRowIndex(0);
		for (T_TradeItemUniquePtr const & item : m_TradeBook->GetData())
		{
			C_TradeItemMarket const * const itemMarket (dynamic_cast<C_TradeItemMarket const *>(item.get()));
			if (itemMarket != nullptr)
			{
				if (itemMarket->IsFeeValid())
				{
					if (itemMarket->GetInputFileLine() == selectedFileLine)
					{
						if (rowIndex == m_GridFees->GetNumberRows())
						{
							rowIndex = gridFeeRowIndex;
							break;
						}

					}
					gridFeeRowIndex++;
				}
			}
		}

		if (rowIndex != m_GridFees->GetNumberRows())
		{
			wxGridCellCoords const topLeft(rowIndex, 0);
			wxGridCellCoords const bottomRight(rowIndex, 1);
			m_GridFees->SelectBlock(topLeft, bottomRight);
			m_GridFees->MakeCellVisible(topLeft);
			m_GridFees->MakeCellVisible(bottomRight);
		}
	}
}

void C_MainFrame::LoadTaxCurrencySettings()
{
	m_TaxCurrencySettings->LoadFile(GetTaxCurrencySettingsDefaultFilePath());
}

void C_MainFrame::SaveTaxCurrencySettings()
{
	wxString const & pathUserDataDir (wxStandardPaths::Get().GetUserDataDir());
	std::filesystem::path dir (pathUserDataDir.ToStdString());
	if (EnsureDirExists(dir))
	{
		m_TaxCurrencySettings->WriteFile(GetTaxCurrencySettingsDefaultFilePath());
	}
	else
	{
		wxString errorStr(wxString::Format("Failed to create user data directory '%s'", pathUserDataDir.c_str()));
		wxMessageDialog * dial = new wxMessageDialog(nullptr, errorStr, wxT("Error"), wxOK | wxICON_ERROR);
		dial->ShowModal();
	}
}

C_CurrencyValue C_MainFrame::ConvertToTaxCurrency(C_CurrencyValue const & value, time_t const time) const
{
	C_CurrencyValue retVal (value);

	if (!m_TaxCurrencySettings->GetCurrencyType().empty())
	{
		retVal = m_TaxCurrencySettings->ConvertToTaxCurrency(value, GetYearFromTimeT(time));
	}

	return retVal;
}

C_CurrencyValue C_MainFrame::ConvertToTaxCurrency(C_CurrencyValue const & value, time_t const time, C_Decimal & rate) const
{
	C_CurrencyValue retVal (value);

	if (m_TaxCurrencySettings->GetCurrencyType().empty())
	{
		rate = C_Decimal(1.f, 0);	//Tax currency is not specified, keep the original value and rate 1.0
	}
	else
	{
		retVal = m_TaxCurrencySettings->ConvertToTaxCurrency(value, GetYearFromTimeT(time), rate);
	}

	return retVal;
}
