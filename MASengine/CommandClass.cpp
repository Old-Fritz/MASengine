////////////////////////////////////////////////////////////////////////////////
// Filename: CommandClass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "CommandClass.h"

CommandClass::CommandClass()
{
}
CommandClass::CommandClass(const CommandClass&)
{

}
CommandClass::~CommandClass()
{

}

void CommandClass::Initialize(const std::string& strCommand)
{
	std::string tempStr;
	std::vector<std::string> tempArr;
	m_commandsNum = 0;
	for (int i = 0; i < strCommand.size(); i++)
	{
		// if symbol is " " then this is the end of the word and param
		if (strCommand[i] == ' ')
		{
			if (tempStr != "") // if string is clear then that means double " " or " " in start or so on and don`t need to adding
				tempArr.emplace_back(tempStr);
			tempStr.clear();
		}
		// if symbol is ";" then this is the end of the command
		else if (strCommand[i] == ';')
		{
			if (tempStr != "") // if string is clear then that means double " " or " " in start or so on and don`t need to adding
				tempArr.emplace_back(tempStr);
			tempStr.clear();
			m_commands.emplace_back(tempArr);
			m_commandsNum++;
			tempArr.clear();
		}
		else
			tempStr += strCommand[i];
	}
	if (m_commandsNum == 0)// if no normal command in the file
	{
		tempArr.clear();
		tempArr.emplace_back("Nothing");
		m_commands.emplace_back(tempArr);
		m_commandsNum++;
	}
}
void CommandClass::Shutdown()
{
	for (int i = 0; i < m_commandsNum; i++)
	{
		m_commands[i].clear();
	}
	m_commands.clear();
}

int CommandClass::getCommandsNum()
{
	return m_commandsNum;
}

int CommandClass::getParamsNum(int commandNumber)
{
	return commandNumber < m_commandsNum ? m_commands[commandNumber].size() : 0;
}
 std::string CommandClass::getParam(int commandNumber, int ParamNum)
{
	//if all indexes in array then give param element
	if (commandNumber < m_commandsNum)
	{
		if (ParamNum < m_commands[commandNumber].size())
			return makeChanges(m_commands[commandNumber][ParamNum]);
		else
			return "";
	}
	else
		return "";

}

 std::string CommandClass::getInitParam(int commandNumber, int ParamNum)
 {
	 if (commandNumber < m_commandsNum)
	 {
		 if (ParamNum < m_commands[commandNumber].size())
			 return m_commands[commandNumber][ParamNum];
		 else
			 return "";
	 }
	 else
		 return "";
 }


void CommandClass::addChange(const std::string& key, const std::string& value)
{
	auto change = m_changes.find(key);
	if (change == m_changes.end())
		m_changes.emplace(std::pair<std::string, std::string>(key, value));
	else
		change->second = value;
}

void CommandClass::addChange(const std::string& key, float value)
{
	addChange(key, std::to_string(value));
}

void CommandClass::addChange(const std::string& key, int value)
{
	addChange(key, std::to_string(value));
}


std::string CommandClass::makeChanges(const std::string & param)
{
	std::string newParam = param;

	//do for all changes
	for (auto change = m_changes.begin();change != m_changes.end(); change++)
	{
		//const std::string& findstr = m_changes[i].first;
		//float value = m_changes[i].second;
		int index;

		//replace all keys on param
		while ((index = newParam.find(change->first)) != std::string::npos)
			newParam.replace(index, change->first.size(), change->second);

	}
	//check, if this is expression code symbol and calculate
	if (newParam[newParam.size() - 1] == '_')
	{
		newParam.erase(newParam.size() - 1);
		double result = Calculator.calculate(newParam);
		newParam = std::to_string(result);
	}

	return newParam;
}

void CommandClass::shareChanges(CommandClass * command)
{
	for (auto change = m_changes.begin();change != m_changes.end(); change++)
	{
		command->addChange(change->first, change->second);
	}
}

