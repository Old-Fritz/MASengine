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
			::operator delete(m_commands.begin()->second.begin()->second, sizeof(m_commands.begin()->second.begin()->second),2);
			m_commands.begin()->second.erase(m_commands.begin()->second.begin());
		}
		m_commands.erase(m_commands.begin());
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

bool CommandManagerClass::addCommand(const std::string & name, const std::string & filename)
{
	bool result;

	long long filenameHash = ModManagerClass::getI().getHash(filename);
	long long nameHash = ModManagerClass::getI().getHash(name);
	//pair<string, map<string, string>*> commandGroup;
	auto commandGroup = m_commands.find(filenameHash);
	if (commandGroup != m_commands.end()) //check for existing group
	{
		auto command = commandGroup->second.find(nameHash);
		if (command != commandGroup->second.end()) //check for existing od command
		{
			m_commandsQueue.push(command->second);
		}
		else //add new command in group
		{
			CommandClass* newCommand = new CommandClass;
			if (!newCommand)
				return false;
			newCommand->Initialize(getTextFromFile(name, filename));
			commandGroup->second.insert(std::pair<long long, CommandClass*>(nameHash, newCommand));
			m_commandsQueue.push(newCommand);
		}
	}

	else //add new group
	{
		//create new command
		CommandClass* newCommand = new CommandClass;
		if (!newCommand)
			return false;
		newCommand->Initialize(getTextFromFile(name, filename));
		//insert it
		std::map<long long, CommandClass*> newCommandGroup; 
		newCommandGroup.insert(std::pair<long long, CommandClass*>(nameHash, newCommand));
		m_commands.insert(std::pair<long long, std::map<long long, CommandClass*>>(filenameHash, newCommandGroup));
		m_commandsQueue.push(newCommand);
	}

	return true;
	//return getTextFromFile(commandName, commandFilename);
}

 std::string  CommandManagerClass::getTextFromFile(const std::string & name, const std::string & filename)
{
	std::ifstream file;
	file.open(ModManagerClass::getI().getDirectory(ModManagerClass::getI().getHash(filename)) + filename);
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
