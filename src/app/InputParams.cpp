//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021-2022 Karel Hala
// License:	  MIT
//*********************************************************************************************************************

#include "InputParams.h"


//*********************************************************************************************************************
//*********************************************************************************************************************


C_InputParams::C_InputParams()
{}

//*********************************************************************************************************************

bool C_InputParams::Read(int argc, char * argv[])
{
	bool retVal (false);

	if (argc != 2)
	{
		m_ErrorMsg += "Bad params count!\n";
		
		m_ErrorMsg += "Help: ";
		char const * programName = strrchr(argv[0], '/');
		if (programName == nullptr)
		{
			programName = strrchr(argv[0], '\\');
		}

		if (programName != nullptr)
		{
			programName++;	//skip the slash
		}
		else
		{
			programName = argv[0];
		}

		m_ErrorMsg += programName;
		m_ErrorMsg += " filename";

	}
	else
	{
		m_DataFileName = argv[1];
		retVal = true;
	}

	return retVal;
}


//*********************************************************************************************************************
//*********************************************************************************************************************
