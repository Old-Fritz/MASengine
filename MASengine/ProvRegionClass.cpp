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
	//find index of prov
	ProvClass* prov = ProvManagerClass::getI().getProv(provID);
	for (auto pr = m_provs.begin();pr != m_provs.end();pr++)
	{
		//return if found prov
		if (*pr == prov)
			return;
	}

	m_provs.emplace_back(prov);
}

void ProvRegionClass::erase(int provID)
{
	//find index of prov
	ProvClass* prov = ProvManagerClass::getI().getProv(provID);
	for (auto pr = m_provs.begin();pr != m_provs.end();pr++)
	{
		//delete if found prov
		if (*pr == prov)
		{
			m_provs.erase(pr);
			return;
		}
	}
}

int ProvRegionClass::getID()
{
	return m_id;
}

std::vector<ProvClass*>* ProvRegionClass::getProvs()
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
