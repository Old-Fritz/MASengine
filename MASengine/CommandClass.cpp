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
		// if symbol is " " then this is the end of the command
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
	if (commandNumber < m_commandsNum)
	{
		return m_commands[commandNumber].size();
	}
	else
		return 0;
}
 std::string CommandClass::getParam(int commandNumber, int ParamNum)
{
	//if all indexes in array then give param element
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


void CommandClass::addChange(const std::string& key, float value)
{
	m_changes.emplace_back(std::pair<std::string, float>(key, value));
}

void CommandClass::makeChanges()
{
	//do for all changes
	for (int i = 0; i < m_changes.size(); i++)
	{
		const std::string& findstr = m_changes[i].first;
		float value = m_changes[i].second;
		int index;
		//do for all commands
		for (int j = 0; j < m_commandsNum; j++)
		{
			//do for all params
			for (int k = 0; k < m_commands[j].size(); k++)
			{
				//replace all keys on param
				while ((index = m_commands[j][k].find(findstr)) != std::string::npos)
					m_commands[j][k].replace(index, findstr.size(), std::to_string(value));
			}
		}
	}
	//check, if this is expression cod symbol and calculate
	for (int j = 0; j < m_commandsNum; j++)
	{
		//do for all params
		for (int k = 0; k < m_commands[j].size(); k++)
		{
			if (m_commands[j][k][m_commands[j][k].size() - 1] == '_')
			{
				m_commands[j][k].erase(m_commands[j][k].size() - 1);
				double result = Calculator.calculate(m_commands[j][k]);
				m_commands[j][k] = std::to_string(result);
			}
		}
	}
}
