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
	m_name = "ProvName"  + std::to_string(m_id) ;

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

bool ProvClass::readFromFile(std::ifstream * file)
{
	std::string temp;

	*file >> temp >> temp; //ID: {
	*file >> temp >> temp; //coords = 
	*file >> m_coords.x >> m_coords.y >> m_coords.z;
	*file >> temp; //}

	return true;
}
