#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <stdarg.h>
#include <time.h>
#include <corecrt.h>
#include <corecrt_wtime.h>

#define LOGGER GTPNet::NetLog::GetInstance()

namespace GTPNet
{

	class NetLog
	{
	public:

		// Log message by string
		void Log(const std::string& _logMessage);

		void Log(const char* _logFormat, ...);

		// overload << operator to log a message
		NetLog& operator<<(const std::string& _logMessage);

		static NetLog* GetInstance();

		const std::string GetCurrentDateTime();

	private:

		NetLog() {};

		NetLog(const NetLog&) {};

		NetLog& operator=(const NetLog&) { return *this; }

		static const std::string m_fileName;

		static std::ofstream m_logFile;

		static NetLog* m_logInstance;
	};
}
