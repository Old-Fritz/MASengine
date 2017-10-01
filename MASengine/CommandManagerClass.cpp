#include "CommandManagerClass.h"

CommandManagerClass* CommandManagerClass::m_instance = 0;

CommandManagerClass::CommandManagerClass()
{
}
CommandManagerClass::CommandManagerClass(const CommandManagerClass &)
{
}
CommandManagerClass::~CommandManagerClass()
{
}

bool CommandManagerClass::Initialize()
{
	return true;
}
void CommandManagerClass::Shutdown()
{
	while (!m_commands.empty())
	{
		while (!m_commands.begin()->second.empty())
		{
			m_commands.begin()->second.begin()->second->Shutdown(); //delete all commands
			::operator delete(m_commands.begin()->second.begin()->second, sizeof(CommandClass),2);
			m_commands.begin()->second.erase(m_commands.begin()->second.begin());
		}
		m_commands.erase(m_commands.begin());
	}

	if (m_instance)
	{
		::operator delete(m_instance, sizeof(*m_instance), 1);
		m_instance = 0;
	}
}

CommandManagerClass & CommandManagerClass::getI()
{
	if (!m_instance)
		m_instance = new(1) CommandManagerClass;
	return *m_instance;
}

bool CommandManagerClass::isFull()
{

	return m_commandsQueue.size() > 0 ? true : false;
}
CommandClass* CommandManagerClass::nextCommand()
{
	//return next command and delete it from queue
	CommandClass* retValue = m_commandsQueue.front();
	m_commandsQueue.pop();
	return retValue;
}

bool CommandManagerClass::addCommand(const std::string & name, PathClass* filename)
{
	bool result;

	int filenameHash = filename->getHash();
	int nameHash = Utils::getHash(name);

	auto commandGroup = m_commands.find(filenameHash);
	//add new group if not existing
	if (commandGroup == m_commands.end()) //check for existing group
	{
		std::map<int, CommandClass*> newCommandGroup;
		m_commands.insert(std::pair<int, std::map<int, CommandClass*>>(filenameHash, newCommandGroup));
		commandGroup = m_commands.find(filenameHash); //find again
	}

	auto command = commandGroup->second.find(nameHash);
	//add new command if not existing
	if (command == commandGroup->second.end()) //check for existing od command
	{
		CommandClass* newCommand = new(4) CommandClass;
		if (!newCommand)
			return false;
		newCommand->Initialize(Utils::getTextFromFile(name, filename->getPath()));
		commandGroup->second.insert(std::pair<int, CommandClass*>(nameHash, newCommand));
		command = commandGroup->second.find(nameHash); //find again
	}

	// add new command in queue
	m_commandsQueue.push(command->second);

	return true;
}

void CommandManagerClass::addChange(const std::string & key, float value)
{
	m_commandsQueue.back()->addChange(key, value);
}
