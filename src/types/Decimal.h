//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
// License:	  
//*********************************************************************************************************************

#pragma once
#include <cstdint>
#include <sstream>

class C_Decimal
{
public:
	typedef int8_t T_E;
	typedef int64_t T_Value;

public:
	C_Decimal() : m_E(0), m_Value(0) {}
	C_Decimal(T_Value const value) : m_E(0), m_Value(value) {}
	C_Decimal(T_Value const value, T_E const e) : m_E(e), m_Value(value) {}

	C_Decimal operator -(C_Decimal const & other) const;
	C_Decimal operator -=(C_Decimal const & other);

	C_Decimal operator +=(C_Decimal const & other);

	C_Decimal operator *(C_Decimal const & other) const;
	C_Decimal operator *=(C_Decimal const & other);
	
	bool operator <=(C_Decimal const & other) const;

	T_E	GetE() const { return m_E; }
	
	void Normalize(T_E const & e);

	bool IsZero() const { return m_Value == 0; }
	void Zero() { m_Value = 0; }

	void PrintData(std::ostringstream & str) const;

private:
	T_E				m_E;
	T_Value			m_Value;
};
