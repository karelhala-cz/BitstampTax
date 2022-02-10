//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021-2022 Karel Hala
// License:	  MIT
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
