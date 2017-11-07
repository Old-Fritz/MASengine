#include "CommandManagerClass.h"

CommandManagerClass* CommandManagerClass::m_instance = 0;

CommandManagerClass::CommandManagerClass()
{
	///INIT COMMAND MAP
	m_commandMap.emplace(std::pair<int,command>(Utils::getHash("updateInterface"), updateInterface));
	//Updating params of element
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("setElVisible"), setElVisible));
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("startDrag"), startDrag));
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("endDrag"), endDrag));
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("elPos"), elPos));
	// Updating params of bitmaps
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("BMposX"), BMposX));
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("BMposY"), BMposY));
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("BMvertPercent"), BMvertPercent));
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("BMhorPercent"), BMhorPercent));
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("BMtranspar"), BMtranspar));
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("BMselIntens"), BMselIntens));
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("BMselCol"), BMselCol));
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("setBMvisible"), setBMvisible));
	//Updating params of strings
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("TSposX"), TSposX));
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("TSposY"), TSposY));
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("TStext"), TStext));
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("TScolor"), TScolor));
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("TSadding"), TSadding));
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("setTvisible"), setTvisible));
	//Special slider commands
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("nextSector"), nextSector));
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("backSector"), backSector));
	//Special list commands
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("addElement"), addElement));
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("deleteElement"), deleteElement));
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("setLastElement"), setLastElement));
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("clearElement"), clearElement));
	//another commands
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("setBMCommand"), setBMCommand));

	///
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("updateGraphics"), updateGraphics));
	//
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("cameraPosition"), cameraPosition));
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("cameraRotation"), cameraRotation));
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("pick"), pick));
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("unPick"), unPick));

	///
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("updateSystem"), updateSystem));
	//
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("position"), position));
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("rotation"), rotation));
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("setButCommand"), setButCommand));
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("setWheelCommand"), setWheelCommand));
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("setMoving"), setMoving));

	///
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("reboot"), reboot));

	///
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("stop"), stop));

	///
	m_commandMap.emplace(std::pair<int, command>(Utils::getHash("nothing"), nothing));

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

CommandManagerClass::command CommandManagerClass::getCommandEnum(int hash)
{
	auto com = m_commandMap.find(hash);
	if (com != m_commandMap.end())
		return com->second;
	else
		return nothing;
}
