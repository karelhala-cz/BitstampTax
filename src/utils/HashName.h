//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
// License:	  
//*********************************************************************************************************************

#pragma once
#include <functional>

#ifdef _DEBUG
#	include <string>
#endif //_DEBUG

class C_HashName
{
public:
	typedef std::uint64_t T_Hash;

	constexpr static T_Hash const PRIME = 0x100000001B3ull;
	constexpr static T_Hash const BASIS = 0xCBF29CE484222325ull;

	constexpr static T_Hash const ComputeHashCompileTime(char const * const str, T_Hash const lastValue = BASIS)
	{
		return *str ? ComputeHashCompileTime(str + 1, (*str ^ lastValue) * PRIME) : lastValue;
	}

	static T_Hash ComputeHash(char const * str)
	{
		T_Hash retVal{ BASIS };

		while (*str)
		{
			retVal ^= *str;
			retVal *= PRIME;
			str++;
		}

		return retVal;
	}

public:
	C_HashName();
	C_HashName(T_Hash const & hash);
	C_HashName(char const * const str);

	T_Hash	GetHash() const { return m_Hash; }

	bool	operator ==(T_Hash const & hash) const { return m_Hash == hash; }
	bool	operator ==(C_HashName const & hashName) const { return m_Hash == hashName.m_Hash; }
	bool	operator ==(char const * const str) const { return *this == ComputeHash(str); }
	bool	operator <(C_HashName const & hashname) const { return m_Hash < hashname.m_Hash; }
	bool	operator >(C_HashName const & hashname) const { return m_Hash > hashname.m_Hash; }

#	ifdef _DEBUG
	std::string	const & GetStr() const { return m_Str; }
#	endif //_DEBUG

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(m_Hash);

#		ifdef _DEBUG
		archive(m_Str);
#		endif //_DEBUG

	}

private:
	static T_Hash const	INVALID_HASH;

	T_Hash m_Hash;

#	ifdef _DEBUG
	std::string	m_Str;
#	endif //_DEBUG
};

namespace std {
	template <>
	struct hash<C_HashName>
	{
	public:
		C_HashName::T_Hash operator()(C_HashName const & hashName) const
		{
			return hashName.GetHash();
		}
	};
}
