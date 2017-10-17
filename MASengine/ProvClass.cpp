#include "ProvClass.h"

ProvClass::ProvClass()
{
}
ProvClass::ProvClass(const ProvClass &)
{
}
ProvClass::~ProvClass()
{
}

bool ProvClass::Initialize(std::ifstream * file, int id)
{
	bool result;

	m_id = id;

	m_layers = new(1) LayersClass;
	if (!m_layers)
	{
		return false;
	}

	result = readFromFile(file);
	if (!result)
	{
		return false;
	}

	//generate base provname
	m_name = "ProvName" + std::to_string(m_id) ;
	
	return true;
}

void ProvClass::Shutdown()
{
	if (m_layers)
	{
		::operator delete(m_layers, sizeof(*m_layers), 1);
		m_layers = 0;
	}
}

int ProvClass::getID()
{
	return m_id;
}

std::string ProvClass::getName()
{
	return m_name;
}

D3DXVECTOR3 ProvClass::getCoords()
{
	return m_coords;
}

LayersClass * ProvClass::getLayers()
{
	return m_layers;
}

void ProvClass::addRegion(GlobalManagerClass::regionType type, int regionID)
{
	switch (type)
	{
	case GlobalManagerClass::BASE:
		m_baseRegion.emplace_back(regionID);
		break;
	case GlobalManagerClass::BLOCK:
		m_blockRegion.emplace_back(regionID);
		break;
	case GlobalManagerClass::NATION:
		m_nationRegion.emplace_back(regionID);
		break;
	default:
		break;
	}
}

void ProvClass::deleteRegion(GlobalManagerClass::regionType type, int regionID)
{
	switch (type)
	{
	case GlobalManagerClass::BASE:
		erase(m_baseRegion, regionID);
		break;
	case GlobalManagerClass::BLOCK:
		erase(m_blockRegion, regionID);
		break;
	case GlobalManagerClass::NATION:
		erase(m_nationRegion, regionID);
		break;
	default:
		break;
	}
}

std::vector<int> ProvClass::getRegions(GlobalManagerClass::regionType type)
{
	switch (type)
	{
	case GlobalManagerClass::BASE:
		return m_baseRegion;
	case GlobalManagerClass::BLOCK:
		return m_blockRegion;
	case GlobalManagerClass::NATION:
		return m_nationRegion;
	default:
		return m_baseRegion;
	}
}


bool ProvClass::readFromFile(std::ifstream * file)
{
	std::string temp;

	*file >> temp >> temp; //ID: {
	*file >> temp >> temp; //coords = 
	*file >> m_coords.x >> m_coords.y >> m_coords.z;
	*file >> temp; //}

	return true;
}

void ProvClass::erase(std::vector<int> &vec, int value)
{
	for (auto i = vec.begin();i != vec.end();i++)
		if (*i == value)
		{
			vec.erase(i);
			return;
		}
}
