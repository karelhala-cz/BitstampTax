//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021-2022 Karel Hala
// License:	  MIT
//*********************************************************************************************************************

#include "AboutDialog.h"
#include "Ids.h"
#include "Version.h"

C_AboutDialog::C_AboutDialog()
	: wxDialog(nullptr, ID_AboutDialog, "About BitstampTax"/*, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER*/)
{
	wxBoxSizer * const topSizer = new wxBoxSizer(wxVERTICAL);

	static int const BORDER(20);
	static int const TEXT_WIDTH(512);
	static int const PROPORTION(0); //0 for unstretchable

	wxString const strProgramVersion(wxString::Format(wxT(" % s v%d.%d.%d"), PROGRAM_NAME, VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH));
	wxStaticText * const programVersion (new wxStaticText(this, wxID_ANY, strProgramVersion, wxDefaultPosition, wxSize(TEXT_WIDTH, 24)));
	topSizer->Add(programVersion, PROPORTION, wxEXPAND | wxALL, BORDER);

	wxString const strAbout(wxT("   The application computes annual profit/loss based on Bitstamp's transaction history. The FIFO method is used. Keep in mind that complete data are needed for proper results."));
	wxStaticText * const aboutProgram (new wxStaticText(this, wxID_ANY, strAbout, wxDefaultPosition, wxSize(TEXT_WIDTH, 80)));
	topSizer->Add(aboutProgram, PROPORTION, wxEXPAND | wxALL, BORDER);

	wxString const strAuthor(wxT("Author:\n   Karel Hála, hala.karel@gmail.com"));
	wxStaticText * const aboutAuthor (new wxStaticText(this, wxID_ANY, strAuthor));
	topSizer->Add(aboutAuthor, PROPORTION, wxEXPAND | wxALL, BORDER);

	wxString const strLicense(wxT("License:\n   MIT"));
	wxStaticText * const license (new wxStaticText(this, wxID_ANY, strLicense, wxDefaultPosition, wxSize(TEXT_WIDTH, 48)));
	topSizer->Add(license, PROPORTION, wxEXPAND | wxALL, BORDER);

	wxString const strDisclaimer(wxT("Disclaimer:\n   Use the software at your own risk."));
	wxStaticText * const disclaimer (new wxStaticText(this, wxID_ANY, strDisclaimer));
	topSizer->Add(disclaimer, PROPORTION, wxEXPAND | wxALL, BORDER);

	wxBoxSizer * const hbox = new wxBoxSizer(wxHORIZONTAL);
	wxButton * const okButton = new wxButton(this, wxID_OK, wxT("Ok"));
	hbox->Add(okButton);

	topSizer->Add(hbox, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, BORDER);

	SetSizerAndFit(topSizer);
}