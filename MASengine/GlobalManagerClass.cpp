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

	std::ifstream file;
	file.open(filepath);
	if (!file.is_open())
		return false;

	std::string logFilepath;
	std::string modFilename;
	std::string settingsFilename;
	int stackSize;
	int tempSize;
	int oneFrameSize;
	int poolSize;

	file >> logFilepath >> modFilename >> settingsFilename;
	file >> stackSize >> tempSize >> oneFrameSize >> poolSize;

	//Init LogManager
	result = LogManager.Initialize(logFilepath);
	if(!result)
	{
		return false;
	}
	LogManager.addLog("LogManager Initialized");


	//Init MemoryManager
	result = MemoryManager.Initialize(stackSize, tempSize, oneFrameSize, poolSize);
	if (!result)
	{
		LogManager.addLog("Error 6-1");
		return false;
	}
	LogManager.addLog("MemoryManager Initialized");

	//Init ModManager
	result = ModManager.Initialize(modFilename);
	if (!result)
	{
		LogManager.addLog("Error 6-2");
		return false;
	}
	LogManager.addLog("ModManager Initialized");

	//Init Settings
	result = Settings.Initialize(settingsFilename);
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
