//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
// License:	  
//*********************************************************************************************************************

#pragma once
#include "CurrencyType.h"
#include "types/Decimal.h"
#include <cstdint>
#include <memory>
#include <sstream>

class C_CurrencyValue
{
public:
	typedef int8_t T_E;
	typedef int64_t T_Value;

public:
	C_CurrencyValue() {}
	C_CurrencyValue(T_CurrencyType const type) : m_Type(type) {}
	C_CurrencyValue(T_CurrencyType const type, T_Value const value) : m_Type(type), m_Value(value) {}
	C_CurrencyValue(T_CurrencyType const type, T_Value const value, T_E const e) : m_Type(type), m_Value(value, e) {}
	C_CurrencyValue(T_CurrencyType const type, C_Decimal const & value) : m_Type(type), m_Value(value) {}

	C_CurrencyValue operator -(C_CurrencyValue const & other) const;
	C_CurrencyValue operator -=(C_CurrencyValue const & other);

	C_CurrencyValue operator +=(C_CurrencyValue const & other);

	C_CurrencyValue operator *(C_Decimal const & other) const;
	
	bool operator <(C_CurrencyValue const & other) const;
	bool operator >(C_CurrencyValue const & other) const;
	bool operator ==(C_CurrencyValue const & other) const;
	bool operator <=(C_CurrencyValue const & other) const;

	T_CurrencyType	GetType() const { return m_Type; }
	C_Decimal const & GetValue() const { return m_Value; }
	
	bool IsZero() const { return m_Value.IsZero(); }
	void Zero() { m_Value.Zero(); }

	std::string GetAsString() const;
	void PrintData(std::ostringstream & str) const;

private:
	T_CurrencyType	m_Type;
	C_Decimal		m_Value;
};

typedef std::unique_ptr<C_CurrencyValue const> T_CurrencyValueConstPtr;
