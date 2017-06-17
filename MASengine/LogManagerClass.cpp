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

	m_file.open(filepath + "/LOG" + getTime() + ".txt");

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

std::string LogManagerClass::getTime()
{
	//get time and data from system
	char dateStr[9];
	char timeStr[9];
	_strdate_s(dateStr);
	_strtime_s(timeStr);
	dateStr[2] = '-';
	dateStr[5] = '-';
	timeStr[2] = '-';
	timeStr[5] = '-';
	
	return (std::string)dateStr + "_" + (std::string)timeStr;
}
