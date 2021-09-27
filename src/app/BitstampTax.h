//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
//*********************************************************************************************************************

#pragma once

#include <wx/wx.h>

class C_MainFrame;


class C_BitstampTax : public wxApp
{
public:
    virtual bool OnInit();

private:
    C_MainFrame *   m_MainFrame;
};
