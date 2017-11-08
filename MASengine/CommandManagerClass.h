////////////////////////////////////////////////////////////////////////////////
// Filename: CommandManagerClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _COMMANDMANAGERCLASS_H_
#define _COMMANDMANAGERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include "CommandClass.h"
#include "PathManagerClass.h"
#include <queue>
#include <fstream>
#include <unordered_map>

////////////////////////////////////////////////////////////////////////////////
// Class name: CommandManagerClass
////////////////////////////////////////////////////////////////////////////////
class CommandManagerClass
{
public:
	enum command {
		updateInterface,
		setElVisible, startDrag, endDrag, elPos, //Updating params of element
		BMposX, BMposY, BMvertPercent, BMhorPercent,  
		BMtranspar, BMselIntens, BMselCol, setBMvisible, //Updating params of bitmaps
		TSposX, TSposY, TStext, TScolor, TSadding, setTvisible,  //Updating params of strings
		nextSector, backSector, //Special slider commands
		addElement, deleteElement, setLastElement, clearElement,  //Special list commands
		setBMCommand, //another commands

		updateGraphics, 
		cameraPosition, cameraRotation, pick, unPick,

		updateSystem, 
		position, rotation, setButCommand, setWheelCommand, setMoving,

		reboot,

		stop,

		nothing,

		operators,
		IF,
		
		get,
		getForward, getProvRegionId,
		
		set,
		setProvRegion
	};
public:
	

	bool Initialize();
	void Shutdown();

	static CommandManagerClass& getI();

	bool isFull();
	CommandClass* nextCommand();
	CommandClass* makeSingleCommand(const std::string & name, PathClass* filename);

	bool addCommand(const std::string& name, PathClass* filename);
	void addChange(const std::string& key, float value);

	command getCommandEnum(int hash);
	command getCommandEnum(const std::string& key);

	void addGlobalChange(const std::string& key, const std::string& value);
	void addGlobalChange(const std::string& key, int value);
	void addGlobalChange(const std::string& key, float value);
private:
	void shareGlobal(CommandClass* command);
	CommandClass* loadCommand(const std::string& name, PathClass* filename);
	CommandManagerClass();
	CommandManagerClass(const CommandManagerClass&);
	~CommandManagerClass();
private:
	std::queue<CommandClass*> m_commandsQueue;
	std::map<int, std::map<int, CommandClass*>> m_commands;
	std::map<std::string, std::string> m_globalChanges;
	std::unordered_map<int, command> m_commandMap;

	static CommandManagerClass* m_instance;
};

#endif