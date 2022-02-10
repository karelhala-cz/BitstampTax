//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021-2022 Karel Hala
// License:	  MIT
//*********************************************************************************************************************

#include "HashName.h"

#include <functional>

C_HashName::T_Hash const C_HashName::INVALID_HASH (0);

C_HashName::C_HashName()
	: m_Hash(INVALID_HASH)
{
}

C_HashName::C_HashName(T_Hash const & hash)
	: m_Hash(hash)
{
}

C_HashName::C_HashName(char const * const str)
	: m_Hash(ComputeHash(str))
#ifdef _DEBUG
	, m_Str(str)
#endif // _DEBUG
{
}

