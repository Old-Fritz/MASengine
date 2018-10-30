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

	//clear all regions
	m_baseRegion.clear();
	m_blockRegion.clear();
	m_nationRegion.clear();
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

void ProvClass::addRegion(GM::regionType type, int regionID)
{
	// add different types of vector in case of type
	switch (type)
	{
	case GM::BASE:
		m_baseRegion.emplace_back(regionID);
		break;
	case GM::BLOCK:
		m_blockRegion.emplace_back(regionID);
		break;
	case GM::NATION:
		m_nationRegion.emplace_back(regionID);
		break;
	default:
		break;
	}
}

void ProvClass::deleteRegion(GM::regionType type, int regionID)
{
	// erase in different types of vector in case of type
	switch (type)
	{
	case GM::BASE:
		erase(m_baseRegion, regionID);
		break;
	case GM::BLOCK:
		erase(m_blockRegion, regionID);
		break;
	case GM::NATION:
		erase(m_nationRegion, regionID);
		break;
	default:
		break;
	}
}

std::vector<int> ProvClass::getRegions(GM::regionType type)
{
	// return different types of vector in case of type
	switch (type)
	{
	case GM::BASE:
		return m_baseRegion;
	case GM::BLOCK:
		return m_blockRegion;
	case GM::NATION:
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
	//find and delete value from vector
	for (auto i = vec.begin();i != vec.end();i++)
		if (*i == value)
		{
			vec.erase(i);
			return;
		}
}
