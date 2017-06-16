#include "ModManagerClass.h"

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
		LogManager.addLog("Error 3-1");
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

	return;
}

long long ModManagerClass::getHash(const std::string & str)
{
	const int p = 101;

	long long hash = 0;
	long long p_pow = 1;
	for (int i = 0; i < str.length();i++)
	{
		hash += static_cast<long long>(str[i] * p_pow);
		p_pow *= p;
	}

	return hash;

}

const std::string & ModManagerClass::getDirectory(long long hash)
{
	auto directory = m_modChanges.find(hash);
	return directory->second;
}

void ModManagerClass::loadMod(const std::string & name, const std::string & modDirectory)
{
	//get all filepaths in directory
	std::vector<std::string&> filenames;
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
		m_modChanges.insert(std::pair<long long, std::string >(getHash(filenames[i]), (modDirectory + "/" + name)));
	}
}