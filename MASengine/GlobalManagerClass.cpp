#include "GlobalManagerClass.h"

GlobalManagerClass* GlobalManagerClass::m_instance = 0;

GlobalManagerClass::GlobalManagerClass()
{
}
GlobalManagerClass::GlobalManagerClass(const GlobalManagerClass &)
{
}
GlobalManagerClass::~GlobalManagerClass()
{
}

bool GlobalManagerClass::Initialize(const std::string& filepath)
{
	bool result;

	std::ifstream file;
	file.open(filepath);
	if (!file.is_open())
		return false;

	std::string logFilepath;
	std::string modFilename;
	PathClass* settingsFilename = new(4) PathClass;
	int stackSize;
	int tempSize;
	int oneFrameSize;
	int poolSize;

	file >> logFilepath >> modFilename;
	file >> settingsFilename;
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
	result = ModManagerClass::getI().Initialize(modFilename);
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 6-2");
		return false;
	}
	LogManagerClass::getI().addLog("ModManager Initialized");

	//Init Settings
	result = SettingsClass::getI().Initialize(settingsFilename);
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 6-6");
		return false;
	}
	LogManagerClass::getI().addLog("Settings Initialized");

	//Init CommandManager
	result = CommandManagerClass::getI().Initialize();
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 6-3");
		return false;
	}
	LogManagerClass::getI().addLog("CommandManager Initialized");

	//Init ResourceManager
	result = ResourceManagerClass::getI().Initialize();
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 6-4");
		return false;
	}
	LogManagerClass::getI().addLog("ResourceManager Initialized");

	//Init SystemStateManager
	result = SystemStateManagerClass::getI().Initialize();
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
	SystemStateManagerClass::getI().Shutdown();
	ResourceManagerClass::getI().Shutdown();
	CommandManagerClass::getI().Shutdown();
	SettingsClass::getI().Shutdown();
	ModManagerClass::getI().Shutdown();
	MemoryManagerClass::getI().Shutdown();
	LogManagerClass::getI().Shutdown();
}

GlobalManagerClass & GlobalManagerClass::getI()
{
	if (!m_instance)
		m_instance = new GlobalManagerClass;
	return *m_instance;
}
