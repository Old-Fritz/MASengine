#include "ModManagerClass.h"

ModManagerClass* ModManagerClass::m_instance = 0;

ModManagerClass::ModManagerClass()
{
}
ModManagerClass::ModManagerClass(const ModManagerClass &)
{
}
ModManagerClass::~ModManagerClass()
{
}

bool ModManagerClass::Initialize(const std::string & modDirectory)
{
	//load names of mods
	std::fstream modFile;
	modFile.open(modDirectory + "/mods.txt", std::ios::in);
	if (!modFile.is_open())
	{
		LogManagerClass::getI().addLog("Error 3-1");
		modFile.close();
		return false;
	}

	int modsNum = 0;
	std::string temp;
	modFile >> modsNum;

	for (int i = 0; i < modsNum; i++)
	{
		modFile >> temp;
		m_modNames.emplace_back(temp);
		loadMod(temp, modDirectory);
	}

	modFile.close();
	return true;
}

void ModManagerClass::Shutdown()
{
	while (!m_modChanges.empty())
	{
		m_modChanges.begin()->second.clear();
		m_modChanges.erase(m_modChanges.begin()->first);
	}
		
	while (!m_modNames.empty())
	{
		(*(m_modNames.begin())).clear();
		m_modNames.erase(m_modNames.begin());
	}

	if (m_instance)
	{
		::operator delete(m_instance, sizeof(*m_instance), 1);
		m_instance = 0;
	}

	return;
}

ModManagerClass & ModManagerClass::getI()
{
	if (!m_instance)
		m_instance = new(1) ModManagerClass;
	return *m_instance;
}

void ModManagerClass::regPath(PathClass * path)
{
	path->changePrefix(getDirectory(path->getShortPath())); // change prefix
}

 std::string  ModManagerClass::getDirectory(int hash)
{
	auto directory = m_modChanges.find(hash);
	if (directory != m_modChanges.end())
		return directory->second;
	else
		return "";
}

 std::string ModManagerClass::getDirectory(const std::string & string)
 {
	 return getDirectory(Utils::getHash(string));
 }

void ModManagerClass::loadMod(const std::string & name, const std::string & modDirectory)
{
	//get all filepaths in directory
	std::vector<std::string> filenames;
	for (recursive_directory_iterator i((modDirectory + "/" + name).c_str()), end; i != end; ++i)
	{
		if (!is_directory(i->path()))
		{
			filenames.emplace_back(i->path().string());
		}
	}

	//normalize them
	for (int i = 0; i < filenames.size(); i++)
	{
		//replace all \\ to /
		int index;
		while ((index = filenames[i].find("\\")) != std::string::npos)
			filenames[i].replace(index, 1, "/");
		filenames[i].erase(0, (modDirectory + "/" + name).size());
		transform(filenames[i].begin(), filenames[i].end(), filenames[i].begin(), tolower);
		//add to map of directories
		m_modChanges.insert(std::pair<long long, std::string >(Utils::getHash(filenames[i]), (modDirectory + "/" + name)));
	}
}
