//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
//*********************************************************************************************************************

#pragma once
#include <wx/wx.h>


class C_MainFrame : public wxFrame
{
public:
    C_MainFrame();

private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent & event);
    void OnAbout(wxCommandEvent & event);
};
