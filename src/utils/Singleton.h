//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021-2022 Karel Hala
// License:	  MIT
//*********************************************************************************************************************

#pragma once

#include <memory>

template<typename T>
class C_Singleton
{
public:
	C_Singleton() = delete;
	C_Singleton(C_Singleton const&) = delete;
	void operator=(C_Singleton const &) = delete;

	static T & GetInstance()
	{
		static std::unique_ptr<T> instance (std::make_unique<T>());
		return *instance;
	}
};
