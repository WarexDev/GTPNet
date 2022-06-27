#include "NetLog.h"

const std::string GTPNet::NetLog::m_fileName = "Log.txt";
GTPNet::NetLog* GTPNet::NetLog::m_logInstance = NULL;
std::ofstream GTPNet::NetLog::m_logFile;

namespace GTPNet
{
	void NetLog::Log(const std::string& _logMessage)
	{
		m_logFile << GetCurrentDateTime() << ":\t";
		m_logFile << _logMessage << "\n";
	}

	void NetLog::Log(const char* _logFormat, ...)
	{
		char* sMessage = NULL;
		int nLength = 0;
		va_list args;
		va_start(args, _logFormat);
		//  Return the number of characters in the string referenced the list of arguments.
		// _vscprintf doesn't count terminating '\0' (that's why +1)
		nLength = _vscprintf(_logFormat, args) + 1;
		sMessage = new char[nLength];
		vsprintf_s(sMessage, nLength, _logFormat, args);
		//vsprintf(sMessage, format, args);
		m_logFile << GetCurrentDateTime() << ":\t";
		m_logFile << sMessage << "\n";
		va_end(args);

		delete[] sMessage;
	}

	NetLog& NetLog::operator<<(const std::string& _logMessage)
	{
		m_logFile << "\n" << GetCurrentDateTime() << ":\t";
		m_logFile << _logMessage << "\n";
		return *this;
	}

	// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
	const std::string NetLog::GetCurrentDateTime()
	{
		time_t     now = time(NULL);
		struct tm  tstruct;
		char       buf[80];
		localtime_s(&tstruct, &now);
		strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
		return buf;
	}

	NetLog* NetLog::GetInstance()
	{
		if (m_logInstance == NULL)
		{
			m_logInstance = new NetLog();
			m_logFile.open(m_fileName.c_str(), std::ios::out | std::ios::app);
		}
		return m_logInstance;
	}

}



