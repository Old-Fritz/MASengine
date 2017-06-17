#include "GlobalManagerClass.h"

GlobalManagerClass::GlobalManagerClass()
{
}
GlobalManagerClass::GlobalManagerClass(const GlobalManagerClass &)
{
}
GlobalManagerClass::~GlobalManagerClass()
{
}

bool GlobalManagerClass::Initialize(const std::string & filepath)
{
	bool result;
	//Init LogManager
	result = LogManager.Initialize(filepath);
	if(!result)
	{
		return false;
	}
	LogManager.addLog("LogManager Initialized");

	//Init MemoryManager
	result = MemoryManager.Initialize(0, 0, 0, 0);
	if (!result)
	{
		LogManager.addLog("Error 6-1");
		return false;
	}
	LogManager.addLog("MemoryManager Initialized");

	//Init ModManager
	result = ModManager.Initialize(filepath);
	if (!result)
	{
		LogManager.addLog("Error 6-2");
		return false;
	}
	LogManager.addLog("ModManager Initialized");

	//Init Settings
	result = Settings.Initialize(filepath);
	if (!result)
	{
		LogManager.addLog("Error 6-6");
		return false;
	}
	LogManager.addLog("Settings Initialized");

	//Init CommandManager
	result = CommandManager.Initialize();
	if (!result)
	{
		LogManager.addLog("Error 6-3");
		return false;
	}
	LogManager.addLog("CommandManager Initialized");

	//Init ResourceManager
	result = ResourceManager.Initialize();
	if (!result)
	{
		LogManager.addLog("Error 6-4");
		return false;
	}
	LogManager.addLog("ResourceManager Initialized");

	//Init SystemStateManager
	result = SystemStateManager.Initialize();
	if (!result)
	{
		LogManager.addLog("Error 6-5");
		return false;
	}
	LogManager.addLog("SystemStateManager Initialized");

	

	return true;
}

void GlobalManagerClass::Shutdown()
{
	//Shutdown all managers
	SystemStateManager.Shutdown();
	ResourceManager.Shutdown();
	CommandManager.Shutdown();
	Settings.Shutdown();
	ModManager.Shutdown();
	MemoryManager.Shutdown();
	LogManager.Shutdown();
}
