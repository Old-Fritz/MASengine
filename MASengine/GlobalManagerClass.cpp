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
	result = LogManagerClass::getI().Initialize(logFilepath);
	if(!result)
	{
		return false;
	}
	LogManagerClass::getI().addLog("LogManager Initialized");


	//Init MemoryManager
	result = MemoryManagerClass::getI().Initialize(stackSize, tempSize, oneFrameSize, poolSize);
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 6-1");
		return false;
	}
	LogManagerClass::getI().addLog("MemoryManager Initialized");

	//Init ModManager
	result = ModManager.Initialize(modFilename);
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 6-2");
		return false;
	}
	LogManagerClass::getI().addLog("ModManager Initialized");

	//Init Settings
	result = Settings.Initialize(settingsFilename);
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 6-6");
		return false;
	}
	LogManagerClass::getI().addLog("Settings Initialized");

	//Init CommandManager
	result = CommandManager.Initialize();
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 6-3");
		return false;
	}
	LogManagerClass::getI().addLog("CommandManager Initialized");

	//Init ResourceManager
	result = ResourceManager.Initialize();
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 6-4");
		return false;
	}
	LogManagerClass::getI().addLog("ResourceManager Initialized");

	//Init SystemStateManager
	result = SystemStateManager.Initialize();
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 6-5");
		return false;
	}
	LogManagerClass::getI().addLog("SystemStateManager Initialized");

	

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
	MemoryManagerClass::getI().Shutdown();
	LogManagerClass::getI().Shutdown();
}
