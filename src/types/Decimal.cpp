//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021-2022 Karel Hala
// License:	  MIT
//*********************************************************************************************************************

#include "Decimal.h"
#include <sstream>
#include <cassert>
#include <algorithm>
#include <string>

C_Decimal C_Decimal::operator -(C_Decimal const & other) const
{
	C_Decimal retVal;

	if (IsValid() && other.IsValid())
	{
		T_E const minE (std::min(m_E, other.m_E));

		C_Decimal normalized(*this);
		normalized.Normalize(minE);

		C_Decimal otherNormalized(other);
		otherNormalized.Normalize(minE);

		retVal = C_Decimal(normalized.m_Value - otherNormalized.m_Value, m_E);
	}

	return retVal;
}

C_Decimal C_Decimal::operator -=(C_Decimal const & other)
{
	if (IsValid())
	{
		if (other.IsValid())
		{
			assert(m_E == other.m_E);
	
			m_Value -= other.m_Value;
		}
		else
		{
			Invalidate();
		}
	}
	
	return *this;
}

C_Decimal C_Decimal::operator +=(C_Decimal const & other)
{
	if (IsValid())
	{
		if (other.IsValid())
		{
			C_Decimal otherNormalized(other);

			T_E const minE (std::min(m_E, other.m_E));
			Normalize(minE);
			otherNormalized.Normalize(minE);
	
			m_Value += other.m_Value;
		}
		else
		{
			Invalidate();
		}
	}
	
	return *this;
}

bool C_Decimal::operator <(C_Decimal const & other) const
{
	assert(IsValid() && other.IsValid());
	assert(m_E == other.m_E);

	return m_Value < other.m_Value;
}

bool C_Decimal::operator >(C_Decimal const & other) const
{
	assert(IsValid() && other.IsValid());
	assert(m_E == other.m_E);

	return m_Value > other.m_Value;
}

bool C_Decimal::operator ==(C_Decimal const & other) const
{
	assert(IsValid() && other.IsValid());
	assert(m_E == other.m_E);

	return m_Value == other.m_Value;
}

bool C_Decimal::operator <=(C_Decimal const & other) const
{
	assert(IsValid() && other.IsValid());
	assert(m_E == other.m_E);

	return m_Value <= other.m_Value;
}

C_Decimal C_Decimal::operator *(C_Decimal const & other) const
{
	C_Decimal retVal;

	if (IsValid() && other.IsValid())
	{
		retVal = C_Decimal(m_Value * other.m_Value, m_E + other.m_E);
	}

	return retVal;
}

C_Decimal C_Decimal::operator *=(C_Decimal const & other)
{
	if (IsValid())
	{
		if (other.IsValid())
		{
			assert(other.GetE() == 1);
			m_Value *= other.m_Value;
		}
		else
		{
			Invalidate();
		}
	}

	return *this;
}

int64_t ipow(int64_t const base, int64_t pow)
{
	return base * ((pow > 1) ? ipow(base, --pow) : 1); 
}

void C_Decimal::Normalize(C_Decimal::T_E const & e)
{
	assert(IsValid());

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

std::string C_Decimal::GetAsString() const
{
	std::ostringstream str;
	PrintData(str);
	return str.str();
}

void C_Decimal::PrintData(std::ostringstream & str) const
{
	if (IsValid())
	{
		if (m_E > 0)
		{
			str << m_Value * ipow(10, m_E);
		}
		else if (m_E < 0)
		{
			//deal with a sign first
			int64_t value(m_Value);
			if (value < 0)
			{
				str << "-";
				value = -value;
			}

			int8_t ePositive (-m_E);
			int64_t const tenPowered  (ipow(10, ePositive));
			int64_t const integralPart (value / tenPowered);
			str << integralPart;
			str << ".";
			int64_t const fractionalPart (value - (integralPart * tenPowered));
			for (int64_t tmpTenPowered = tenPowered / 10; tmpTenPowered > 0; tmpTenPowered /= 10)
			{
				if (tmpTenPowered > fractionalPart)
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
	else
	{
		str << "INVALID_NUMBER";
	}
}


bool StringToDecimal(char const * const str, C_Decimal & value)
{
	bool retVal (false);

	C_Decimal::T_E decimalE (0);
	C_Decimal::T_Value decimalValue (0);

	bool decimalPointPassed (false);

	for (unsigned i = 0; str[i] != '\0'; ++i)
	{
		char const ch (str[i]);

		if ((ch >= '0') && (ch <= '9'))
		{
			decimalValue = decimalValue * 10 + ch - '0';

			if (decimalPointPassed)
			{
				decimalE--;
			}

			retVal = true;
		}
		else if ((ch == '.') || (ch == ','))
		{
			if (decimalPointPassed)
			{
				retVal = false;
				break;
			}

			decimalPointPassed = true;
		}
		else
		{
			retVal = false;
			break;
		}
	}

	if (retVal)
	{
		value = C_Decimal(decimalValue, decimalE);
	}

	return retVal;
}

std::istream & operator>> (std::istream & in, C_Decimal & value)
{
	std::string str;
	in >> str;
	StringToDecimal(str.c_str(), value);

	return in;
}

std::ostream & operator<< (std::ostream & out, C_Decimal const & value)
{
	out << value.GetAsString();

	return out;
}