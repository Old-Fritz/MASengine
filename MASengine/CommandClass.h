////////////////////////////////////////////////////////////////////////////////
// Filename: CommandClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _COMMANDCLASS_H_
#define _COMMANDCLASS_H_


//////////////
// INCLUDES //
//////////////

#include "CalculatorClass.h"

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
	void makeChanges();
private:
	int m_commandsNum;
	std::vector<std::vector<std::string>> m_commands;
	std::vector<std::pair<std::string, float>> m_changes;
};

#endif