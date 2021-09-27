//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
//*********************************************************************************************************************

#include "BitstampTax.h"
#include "MainFrame.h"

bool C_BitstampTax::OnInit()
{
    m_MainFrame = new C_MainFrame();
    m_MainFrame->Show();
    
    return true;
}