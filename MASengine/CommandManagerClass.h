////////////////////////////////////////////////////////////////////////////////
// Filename: CommandManagerClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _COMMANDMANAGERCLASS_H_
#define _COMMANDMANAGERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include "CommandClass.h"
#include "ModManagerClass.h"
#include <queue>
#include <fstream>

////////////////////////////////////////////////////////////////////////////////
// Class name: CommandManagerClass
////////////////////////////////////////////////////////////////////////////////
class CommandManagerClass
{
public:
	CommandManagerClass();
	CommandManagerClass(const CommandManagerClass&);
	~CommandManagerClass();

	bool Initialize();
	void Shutdown();

	bool isFull();
	CommandClass* nextCommand();

	bool addCommand(const std::string& name, const std::string& filename);
private:
	const std::string& getTextFromFile(const std::string& name, const std::string& filename);
private:
	std::queue<CommandClass*> m_commandsQueue;
	std::map<long long, std::map<long long, CommandClass*>> m_commands;
};

/////////////
// GLOBALS //
/////////////
static CommandManagerClass CommandManager;

#endif