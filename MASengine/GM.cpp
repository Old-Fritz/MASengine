#include "GM.h"

GM* GM::m_instance = 0;

GM::GM()
{
	///INIT REGION TYPE MAP
	m_regionTypeMap.emplace(std::pair<int, regionType>(Utils::getHash("BASE"), BASE));
	//Updating params of element
	m_regionTypeMap.emplace(std::pair<int, regionType>(Utils::getHash("BLOCK"), BLOCK));
	m_regionTypeMap.emplace(std::pair<int, regionType>(Utils::getHash("NATION"), NATION));
}
GM::GM(const GM &)
{
}
GM::~GM()
{
}

bool GM::Initialize(const std::string& filepath)
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
	m_logManager = new LogManager;
	if (!m_logManager)
		return false;

	result = m_logManager->Initialize(logFilepath);
	if(!result)
		return false;

	GM::LM()->addLog("LogManager Initialized");


	//Init MemoryManager
	result = MemoryManager::getI()->Initialize(stackSize, tempSize, oneFrameSize, poolSize);
	if (!result)
	{
		GM::LM()->addLog("Problem with memory manager initializtion");
		return false;
	}
	GM::LM()->addLog("MemoryManager Initialized");

	//Init ModManager
	result = ModManagerClass::getI().Initialize(modFilename);
	if (!result)
	{
		GM::LM()->addLog("Error 6-2");
		return false;
	}
	GM::LM()->addLog("ModManager Initialized");

	//Init Path Manager
	result = PathManagerClass::getI().Initialize();
	if (!result)
	{
		GM::LM()->addLog("Error 6-7");
		return false;
	}
	GM::LM()->addLog("PathManager Initialized");

	//Init Settings
	settingsFilename->changePath(settingsFilenameStr);
	result = SettingsClass::getI().Initialize(settingsFilename);
	if (!result)
	{
		GM::LM()->addLog("Error 6-6");
		return false;
	}
	GM::LM()->addLog("Settings Initialized");

	//Init CommandManager
	result = CommandManagerClass::getI().Initialize();
	if (!result)
	{
		GM::LM()->addLog("Error 6-3");
		return false;
	}
	GM::LM()->addLog("CommandManager Initialized");

	//Init ResourceManager
	result = ResourceManagerClass::getI().Initialize();
	if (!result)
	{
		GM::LM()->addLog("Error 6-4");
		return false;
	}
	GM::LM()->addLog("ResourceManager Initialized");

	//Init SystemStateManager
	result = SystemStateManagerClass::getI().Initialize();
	if (!result)
	{
		GM::LM()->addLog("Error 6-5");
		return false;
	}
	GM::LM()->addLog("SystemStateManager Initialized");

	

	return true;
}

void GM::Shutdown()
{
	//Shutdown all managers
	SystemStateManagerClass::getI().Shutdown();
	ResourceManagerClass::getI().Shutdown();
	CommandManagerClass::getI().Shutdown();
	SettingsClass::getI().Shutdown();
	PathManagerClass::getI().Shutdown();
	ModManagerClass::getI().Shutdown();
	MemoryManager::getI()->Shutdown();
	GM::LM()->Shutdown();

	if (m_instance)
	{
		::operator delete(m_instance, sizeof(*m_instance), 3);
		m_instance = 0;
	}
}

GM::regionType GM::getRegionTypeEnum(int hash)
{
	auto reg = m_regionTypeMap.find(hash);
	if (reg != m_regionTypeMap.end())
		return reg->second;
	else
		return BASE;
}

GM::regionType GM::getRegionTypeEnum(const std::string & key)
{
	return getRegionTypeEnum(Utils::getHash(key));
}

GM & GM::getI()
{
	if (!m_instance)
		m_instance = new GM;
	return *m_instance;
}

LogManager * GM::LM()
{
	return m_instance->m_logManager;
}
