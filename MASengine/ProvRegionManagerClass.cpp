#include "ProvRegionManagerClass.h"

ProvRegionManagerClass::ProvRegionManagerClass()
{
}
ProvRegionManagerClass::ProvRegionManagerClass(const ProvRegionManagerClass &)
{
}
ProvRegionManagerClass::~ProvRegionManagerClass()
{
}

bool ProvRegionManagerClass::Initialize(const std::string & filename)
{
	bool result;

	int numOfProvs;
	std::ifstream file;
	file.open(filename);

	if (file.fail())
		return false;

	file >> numOfProvs;
	for (int i = 0;i < numOfProvs;i++)
	{
		ProvRegionClass* region = new(1) ProvRegionClass;
		result = region->Initialize(&file, i);
		if (!result)
		{
			return false;
		}
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
}

ProvRegionClass * ProvRegionManagerClass::getProvRegion(int provRegionID)
{
	if (provRegionID < m_provRegions.size())
		return m_provRegions[provRegionID];
	else
		return m_provRegions[0];
}
