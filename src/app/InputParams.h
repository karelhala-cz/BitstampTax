//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
// License:	  
//*********************************************************************************************************************

#pragma once
#include <string>

class C_InputParams
{
public:
	C_InputParams();

	bool Read(int argc, char * argv[]);

	std::string const & GetDataFileName() const { return m_DataFileName; }
	std::string const & GetErrorMsg() const { return m_ErrorMsg; }


private:
	std::string m_DataFileName;

	std::string m_ErrorMsg;
};
