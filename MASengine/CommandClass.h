////////////////////////////////////////////////////////////////////////////////
// Filename: CommandClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _COMMANDCLASS_H_
#define _COMMANDCLASS_H_


//////////////
// INCLUDES //
//////////////

#include "CalculatorClass.h"
#include <map>

////////////////////////////////////////////////////////////////////////////////
// Class name: CommandClass
////////////////////////////////////////////////////////////////////////////////

class CommandClass
{
public:
	CommandClass();
	CommandClass(const CommandClass&);
	~CommandClass();

	void Initialize(const std::string& strCommand);
	void Shutdown();

	int getCommandsNum();
	int getParamsNum(int commandNumber);

	std::string getParam(int commandNumber, int ParamNum);

	//work with changes
	void addChange(const std::string& key, float value);
	std::string makeChanges(const std::string& param);
private:
	int m_commandsNum;
	std::vector<std::vector<std::string>> m_commands;
	std::map<std::string, float> m_changes;
};

#endif