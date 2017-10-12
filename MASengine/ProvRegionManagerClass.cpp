#include "ProvRegionManagerClass.h"

ProvRegionManagerClass* ProvRegionManagerClass::m_instance = 0;

ProvRegionManagerClass::ProvRegionManagerClass()
{
}
ProvRegionManagerClass::ProvRegionManagerClass(const ProvRegionManagerClass &)
{
}
ProvRegionManagerClass::~ProvRegionManagerClass()
{
}

bool ProvRegionManagerClass::Initialize(PathClass* filename)
{
	bool result;

	int numOfProvs;
	std::ifstream file;
	file.open(filename->getPath());

	if (file.fail())
		return false;

	file >> numOfProvs;
	for (int i = 0;i < numOfProvs;i++)
	{
		ProvRegionClass* region = new(4) ProvRegionClass;
		result = region->Initialize(&file, i);
		if (!result)
		{
			return false;
		}
		m_provRegions.emplace_back(region);
	}

	return true;
}

void ProvRegionManagerClass::Shutdown()
{
	for (int i = m_provRegions.size() - 1;i >= 0;i--)
	{
		if (m_provRegions[i])
		{
			m_provRegions[i]->Shutdown();
			::operator delete(m_provRegions[i], sizeof(*m_provRegions[i]), 1);
			m_provRegions[i] = 0;
		}
	}
	m_provRegions.clear();

	if (m_instance)
	{
		::operator delete(m_instance, sizeof(*m_instance), 1);
		m_instance = 0;
	}
}

ProvRegionClass * ProvRegionManagerClass::getProvRegion(int provRegionID)
{
	if (provRegionID < m_provRegions.size())
		return m_provRegions[provRegionID];
	else
		return m_provRegions[0];
}

ProvRegionClass * ProvRegionManagerClass::getSelectedRegion()
{
	return m_selectedRegion;
}

ProvRegionManagerClass & ProvRegionManagerClass::getI()
{
	if (!m_instance)
		m_instance = new(1) ProvRegionManagerClass;
	else
		return *m_instance;
}
