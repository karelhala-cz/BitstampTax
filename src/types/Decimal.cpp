//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
// License:	  
//*********************************************************************************************************************

#include "Decimal.h"
#include <sstream>
#include <cassert>
#include <algorithm>

C_Decimal C_Decimal::operator -(C_Decimal const & other) const
{
	assert(m_E == other.m_E);
	
	return C_Decimal(m_Value - other.m_Value, m_E);
}

C_Decimal C_Decimal::operator -=(C_Decimal const & other)
{
	assert(m_E == other.m_E);
	
	m_Value -= other.m_Value;
	
	return *this;
}

C_Decimal C_Decimal::operator +=(C_Decimal const & other)
{
	C_Decimal otherNormalized(other);

	T_E const minE (std::min(m_E, other.m_E));
	Normalize(minE);
	otherNormalized.Normalize(minE);
	
	m_Value += other.m_Value;
	
	return *this;
}

bool C_Decimal::operator <=(C_Decimal const & other) const
{
	assert(m_E == other.m_E);

	return m_Value <= other.m_Value;
}

C_Decimal C_Decimal::operator *(C_Decimal const & other) const
{
	return C_Decimal(m_Value * other.m_Value, m_E + other.m_E);
}

C_Decimal C_Decimal::operator *=(C_Decimal const & other)
{
/*	C_CurrencyValue otherNormalized (other);

	if (m_E < otherNormalized.m_E)
	{
		otherNormalized.Normalize(m_E);
	}
	else if (m_E > otherNormalized.m_E)
	{
		Normalize(otherNormalized.GetE());
	}

	assert (GetE() == otherNormalized.GetE);*/

	assert(other.GetE() == 1);
	m_Value *= other.m_Value;

	return *this;
}

int64_t ipow(int64_t const base, int64_t pow)
{
	return base * ((pow > 1) ? ipow(base, --pow) : 1); 
}

void C_Decimal::Normalize(C_Decimal::T_E const & e)
{
	while (e < m_E)
	{
		m_E--;
		m_Value *= 10;
	}

	while (e > m_E)
	{
		m_E++;
		assert(m_Value % 10 == 0);
		m_Value /= 10;
	}
}

void C_Decimal::PrintData(std::ostringstream & str) const
{
	if (m_E > 0)
	{
		str << m_Value * ipow(10, m_E);
	}
	else if (m_E < 0)
	{
		int8_t ePositive (-m_E);
		int64_t const integralPart (m_Value / ipow(10, ePositive));
		str << integralPart;
		str << ".";
		int64_t const fractionalPart (m_Value - (integralPart * ipow(10, ePositive)));
		ePositive--;
		for (; ePositive > 0; ePositive--)
		{
			int64_t const testNumber (ipow(10, ePositive));
			if (testNumber > fractionalPart)
			{
				str << "0";
			}
			else
			{
				str << fractionalPart;
				break;
			}
		}
	}
	else
	{
		str << m_Value;
	}
}
