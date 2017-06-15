#include "LogManagerClass.h"


LogManagerClass::LogManagerClass()
{
}
LogManagerClass::LogManagerClass(const LogManagerClass &)
{
}
LogManagerClass::~LogManagerClass()
{
}

bool LogManagerClass::Initialize(const std::string & filepath)
{
	//create file for logging
	m_file.open(filepath + "LOG" + getTime());

	if (!m_file.is_open())
		return false;

	return true;
}
void LogManagerClass::Shutdown()
{
	m_file.close();
}

void LogManagerClass::addLog(const std::string & log)
{
	//add new line in log
	m_file << getTime() + " " + log << std::endl;
}

const std::string& LogManagerClass::getTime()
{
	//get time and data from system
	char dateStr[9];
	char timeStr[9];
	_strdate_s(dateStr);
	_strtime_s(timeStr);
	return (std::string)dateStr + "_" + (std::string)timeStr;
}
