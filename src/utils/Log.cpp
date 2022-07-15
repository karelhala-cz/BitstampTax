//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021-2022 Karel Hala
// License:	  MIT
//*********************************************************************************************************************

#include "Log.h"

#include <wx/msgdlg.h>

#include <assert.h>

C_Log::C_Log()
{
}

C_Log::~C_Log()
{
	m_FileStream.flush();
	m_FileStream.close();
}

void C_Log::Init(std::string const & filename)
{
	m_FileStream.open(filename);
}

void C_Log::Log(std::string const & msg, E_MessageType const type, bool const showDialog)
{
	assert (m_FileStream.is_open());

	switch (type)
	{
		case E_MessageType::Info: 
		{
			m_FileStream << "Info: ";
		}
		break;

		case E_MessageType::Warning: 
		{
			m_FileStream << "Warning: ";
		}
		break;

		case E_MessageType::Error: 
		{
			m_FileStream << "Error: ";
		}
		break;

		default: 
			assert(0);
	}

	if (showDialog)
	{

		wxMessageDialog * const dialog (new wxMessageDialog(nullptr, msg, wxT("Error"), wxOK | wxICON_ERROR));
		dialog->ShowModal();
	}

	m_FileStream << msg << std::endl;

	m_FileStream.flush();
}

void C_Log::LogInfo(std::string const & msg, bool const showDialog /* = false */)
{
	Log(msg, E_MessageType::Info, showDialog);
}

void C_Log::LogWarning(std::string const & msg, bool const showDialog /* = false */)
{
	Log(msg, E_MessageType::Warning, showDialog);
}

void C_Log::LogError(std::string const & msg, bool const showDialog /* = false */)
{
	Log(msg, E_MessageType::Error, showDialog);
}

void C_Log::ShowDialog(std::string const & msg, E_MessageType const type) const
{
	int dialogIconFlag (0);
	wxString caption;

	switch (type)
	{
		case E_MessageType::Info: 
		{
			caption = "Info";
			dialogIconFlag = wxICON_INFORMATION;
		}
		break;

		case E_MessageType::Warning: 
		{
			caption = "Warning";
			dialogIconFlag = wxICON_WARNING;
		}
		break;

		case E_MessageType::Error: 
		{
			caption = "Error";
			dialogIconFlag = wxICON_ERROR;
		}
		break;

		default: 
			assert(0);
	}

	wxMessageDialog * const dial = new wxMessageDialog(nullptr, msg, caption, wxOK | dialogIconFlag);
	dial->ShowModal();
}