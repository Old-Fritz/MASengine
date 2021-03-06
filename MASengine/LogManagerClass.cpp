﻿#include "LogManagerClass.h"

static LogManagerClass* m_instance;

LogManagerClass * LogManagerClass::m_instance = 0;

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
	addLog("Perfect Shutdown! Have a nice day!");
	m_file.close();
}

LogManagerClass& LogManagerClass::getI()
{
	if (!m_instance)
		m_instance = new LogManagerClass();
	return *m_instance;
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
