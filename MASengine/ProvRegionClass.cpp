#include "ProvRegionClass.h"

ProvRegionClass::ProvRegionClass()
{
}
ProvRegionClass::ProvRegionClass(const ProvRegionClass &)
{
}
ProvRegionClass::~ProvRegionClass()
{
}

bool ProvRegionClass::Initialize(std::ifstream * file, int id)
{
	bool result;

	m_id = id;

	result = readFromFile(file);
	if (!result)
	{
		return false;
	}

	return true;
}

void ProvRegionClass::Shutdown()
{
	m_provs.clear();
}

void ProvRegionClass::add(int provID)
{
	m_provs.insert(ProvManagerClass::getI().getProv(provID));
}

void ProvRegionClass::erase(int provID)
{
	m_provs.erase(ProvManagerClass::getI().getProv(provID));
}

int ProvRegionClass::getID()
{
	return m_id;
}

std::set<ProvClass*>* ProvRegionClass::getProvs()
{
	return &m_provs;
}

bool ProvRegionClass::readFromFile(std::ifstream * file)
{
	std::string temp;
	int numOfProvs;
	int provID;

	*file >> temp >> temp; //ID: {
	*file >> temp >> temp; //color = 
	*file >> m_color.x >> m_color.y >> m_color.z >> m_color.w;
	*file >> temp >> temp; //numOfProvs = 
	*file >> numOfProvs;
	*file >> temp; //Provs:
	for (int i = 0;i < numOfProvs;i++)
	{
		*file >> provID;
		add(provID);
	}
	*file >> temp; //}

	return true;
}
