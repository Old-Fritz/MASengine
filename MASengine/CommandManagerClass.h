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
	

	bool Initialize();
	void Shutdown();

	static CommandManagerClass& getI();

	bool isFull();
	CommandClass* nextCommand();

	bool addCommand(const std::string& name, PathClass* filename);
	void addChange(const std::string& key, float value);
private:
	CommandManagerClass();
	CommandManagerClass(const CommandManagerClass&);
	~CommandManagerClass();

	std::string getTextFromFile(const std::string& name, PathClass* filename);
private:
	std::queue<CommandClass*> m_commandsQueue;
	std::map<long long, std::map<long long, CommandClass*>> m_commands;

	static CommandManagerClass* m_instance;
};

#endif