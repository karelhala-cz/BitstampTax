//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021-2022 Karel Hala
// License:	  MIT
//*********************************************************************************************************************

#include "Singleton.h"
#include <fstream>

class C_Log
{
public:
	enum class E_MessageType
	{
		Info,
		Warning,
		Error,

		Last
	};

	C_Log();
	~C_Log();

	void Init(std::string const & filename);

	void Log(std::string const & msg, E_MessageType const type, bool const showDialog);
	void LogInfo(std::string const & msg, bool const showDialog = false);
	void LogWarning(std::string const & msg, bool const showDialog = false);
	void LogError(std::string const & msg, bool const showDialog = false);

private:
	void ShowDialog(std::string const & msg, E_MessageType const type) const;

private:
	std::ofstream m_FileStream;
};

typedef C_Singleton<C_Log> T_AppLog;
