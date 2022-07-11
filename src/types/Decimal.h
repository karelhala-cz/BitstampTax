//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021-2022 Karel Hala
// License:	  MIT
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
	C_Decimal() : m_Valid(false), m_E(0), m_Value(0) {}
	C_Decimal(T_Value const value) : m_Valid(true), m_E(0), m_Value(value) {}
	C_Decimal(T_Value const value, T_E const e) : m_Valid(true), m_E(e), m_Value(value) {}

	C_Decimal operator -(C_Decimal const & other) const;
	C_Decimal operator -=(C_Decimal const & other);

	C_Decimal operator +=(C_Decimal const & other);

	C_Decimal operator *(C_Decimal const & other) const;
	C_Decimal operator *=(C_Decimal const & other);
	
	bool operator <(C_Decimal const & other) const;
	bool operator >(C_Decimal const & other) const;
	bool operator ==(C_Decimal const & other) const;
	bool operator <=(C_Decimal const & other) const;

	friend std::istream & operator>> (std::istream & in, C_Decimal & value);
	friend std::ostream & operator<< (std::ostream & out, C_Decimal const & value);

	T_E	GetE() const { return m_E; }
	
	void Normalize(T_E const & e);

	bool IsValid() const { return m_Valid; }
	void SetValid() { m_Valid = true; }
	void Invalidate() { m_Valid = false; }

	bool IsZero() const { return m_Value == 0; }
	void Zero() { m_Value = 0; }

	std::string GetAsString() const;
	void PrintData(std::ostringstream & str) const;

private:
	bool			m_Valid;
	T_E				m_E;
	T_Value			m_Value;
};

bool StringToDecimal(char const * const str, C_Decimal & value);
