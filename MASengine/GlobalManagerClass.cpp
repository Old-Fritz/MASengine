#include "GlobalManagerClass.h"

GlobalManagerClass* GlobalManagerClass::m_instance = 0;

GlobalManagerClass::GlobalManagerClass()
{
	///INIT REGION TYPE MAP
	m_regionTypeMap.emplace(std::pair<int, regionType>(Utils::getHash("BASE"), BASE));
	//Updating params of element
	m_regionTypeMap.emplace(std::pair<int, regionType>(Utils::getHash("BLOCK"), BLOCK));
	m_regionTypeMap.emplace(std::pair<int, regionType>(Utils::getHash("NATION"), NATION));
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
	std::string settingsFilenameStr;
	PathClass* settingsFilename = new(4) PathClass;
	int stackSize;
	int tempSize;
	int oneFrameSize;
	int poolSize;

	file >> logFilepath >> modFilename;
	file >> settingsFilenameStr;
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

	//Init Path Manager
	result = PathManagerClass::getI().Initialize();
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 6-7");
		return false;
	}
	LogManagerClass::getI().addLog("PathManager Initialized");

	//Init Settings
	settingsFilename->changePath(settingsFilenameStr);
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
	PathManagerClass::getI().Shutdown();
	ModManagerClass::getI().Shutdown();
	MemoryManagerClass::getI().Shutdown();
	LogManagerClass::getI().Shutdown();

	if (m_instance)
	{
		::operator delete(m_instance, sizeof(*m_instance), 3);
		m_instance = 0;
	}
}

GlobalManagerClass::regionType GlobalManagerClass::getRegionTypeEnum(int hash)
{
	auto reg = m_regionTypeMap.find(hash);
	if (reg != m_regionTypeMap.end())
		return reg->second;
	else
		return BASE;
}

GlobalManagerClass::regionType GlobalManagerClass::getRegionTypeEnum(const std::string & key)
{
	return getRegionTypeEnum(Utils::getHash(key));
}

GlobalManagerClass & GlobalManagerClass::getI()
{
	if (!m_instance)
		m_instance = new GlobalManagerClass;
	return *m_instance;
}
