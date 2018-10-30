#include "LogManager.h"

static LogManager* m_instance;

LogManager::LogManager()
{
}
LogManager::LogManager(const LogManager &)
{
}
LogManager::~LogManager()
{
}

bool LogManager::Initialize(const std::string & filepath)
{
	//create file for logging

	m_file.open(filepath + "/LOG" + getTime() + ".txt");

	if (!m_file.is_open())
		return false;

	return true;
}
void LogManager::Shutdown()
{
	addLog("Perfect Shutdown! Have a nice day!");
	m_file.close();
}

void LogManager::addLog(const std::string & log)
{
	//add new line in log
	m_file << getTime() + " " + log << std::endl;
}

std::string LogManager::getTime()
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
