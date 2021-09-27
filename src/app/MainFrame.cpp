//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
//*********************************************************************************************************************

#include "MainFrame.h"
#include "Ids.h"

C_MainFrame::C_MainFrame()
    : wxFrame(NULL, wxID_ANY, "Hello World")
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);
    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");

    Bind(wxEVT_MENU, &C_MainFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &C_MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &C_MainFrame::OnExit, this, wxID_EXIT);


}

void C_MainFrame::OnExit(wxCommandEvent & event)
{
    Close(true);
}

void C_MainFrame::OnAbout(wxCommandEvent & event)
{
    wxMessageBox("This is a wxWidgets Hello World example",
                 "About Hello World", wxOK | wxICON_INFORMATION);
}

void C_MainFrame::OnHello(wxCommandEvent & event)
{
    wxLogMessage("Hello world from wxWidgets!");
}