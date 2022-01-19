//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
//*********************************************************************************************************************

#pragma once

#include <wx/wx.h>

class C_AboutDialog : public wxDialog
{
public:
	C_AboutDialog();

	void OnClose(wxCloseEvent &) { this->Destroy(); } // Ask wxWidgets to `delete this; }

private:
};
