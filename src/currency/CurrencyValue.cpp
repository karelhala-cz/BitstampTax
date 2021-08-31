//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
// License:	  
//*********************************************************************************************************************

#include "CurrencyValue.h"
#include <sstream>
#include <cassert>

C_CurrencyValue C_CurrencyValue::operator -(C_CurrencyValue const & other) const
{
	assert(m_Type == other.m_Type);
	
	return C_CurrencyValue(m_Type, m_Value - other.GetValue());
}

C_CurrencyValue C_CurrencyValue::operator -=(C_CurrencyValue const & other)
{
	assert(m_Type == other.m_Type);
	
	m_Value -= other.GetValue();
	
	return *this;
}

C_CurrencyValue C_CurrencyValue::operator +=(C_CurrencyValue const & other)
{
	assert(m_Type == other.m_Type);

	m_Value += other.GetValue();
	
	return *this;
}

bool C_CurrencyValue::operator <=(C_CurrencyValue const & other) const
{
	assert(m_Type == other.m_Type);

	return m_Value <= other.GetValue();
}

C_CurrencyValue C_CurrencyValue::operator *(C_Decimal const & other) const
{
	return C_CurrencyValue(m_Type, m_Value * other);
}

void C_CurrencyValue::PrintData(std::ostringstream & str) const
{
	m_Value.PrintData(str);
	
	str << " ";

	str << std::string(CurrencyTypeToStr(m_Type));
}
