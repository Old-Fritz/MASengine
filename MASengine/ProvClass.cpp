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
		m_baseRegion.emplace(regionID);
		break;
	case GlobalManagerClass::BLOCK:
		m_blockRegion.emplace(regionID);
		break;
	case GlobalManagerClass::NATION:
		m_nationRegion.emplace(regionID);
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
		m_baseRegion.erase(regionID);
		break;
	case GlobalManagerClass::BLOCK:
		m_blockRegion.erase(regionID);
		break;
	case GlobalManagerClass::NATION:
		m_nationRegion.erase(regionID);
		break;
	default:
		break;
	}
}

std::set<int> ProvClass::getRegions(GlobalManagerClass::regionType type)
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
