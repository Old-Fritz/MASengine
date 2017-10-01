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
	if (m_commandsQueue.size() > 0)
		return true;
	else
		return false;
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

	long long filenameHash = Utils::getHash(filename->getShortPath());
	long long nameHash = Utils::getHash(name);

	auto commandGroup = m_commands.find(filenameHash);
	//add new group if not existing
	if (commandGroup == m_commands.end()) //check for existing group
	{
		std::map<long long, CommandClass*> newCommandGroup;
		m_commands.insert(std::pair<long long, std::map<long long, CommandClass*>>(filenameHash, newCommandGroup));
		commandGroup = m_commands.find(filenameHash); //find again
	}

	auto command = commandGroup->second.find(nameHash);
	//add new command if not existing
	if (command == commandGroup->second.end()) //check for existing od command
	{
		CommandClass* newCommand = new(4) CommandClass;
		if (!newCommand)
			return false;
		newCommand->Initialize(getTextFromFile(name, filename));
		commandGroup->second.insert(std::pair<long long, CommandClass*>(nameHash, newCommand));
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

 std::string  CommandManagerClass::getTextFromFile(const std::string & name, PathClass* filename)
{
	std::ifstream file;
	file.open(filename->getPath());
	std::string temp1 = "}";
	std::string temp2;
	while (file)
	{
		file >> temp2;
		if (temp2 == name)
		{
			if (temp1[temp1.size() - 1] == '}')
			{
				break;
			}
		}
		temp1 = temp2;
	}
	if (temp2 != name)
	{
		file.close();
		return name;
	}
		
	else
	{
		temp1 = "";
		unsigned char a = ' ';
		while (a != '{' && file)
		{
			a = file.get();
		}
		a = file.get();
		while (a != '}' && file)
		{
			if (a < ' ')
				a = ' ';
			temp1 += a;
			a = file.get();
		}
		file.close();
		return temp1;
	}
}
