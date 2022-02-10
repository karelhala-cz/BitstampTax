//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021-2022 Karel Hala
// License:	  MIT
//*********************************************************************************************************************

#include "BitstampTax.h"
#include "MainFrame.h"

wxIMPLEMENT_APP(C_BitstampTax);

bool C_BitstampTax::OnInit()
{
    m_MainFrame = new C_MainFrame();
    m_MainFrame->Show();
    
    return true;
}