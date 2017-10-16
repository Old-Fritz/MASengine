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

void ProvRegionManagerClass::Shutdown()
{
	for (int i = m_provRegions.size() - 1;i >= 0;i--)
	{
		if (m_provRegions[i])
		{
			m_provRegions[i]->Shutdown();
			::operator delete(m_provRegions[i], sizeof(*m_provRegions[i]), 2);
			m_provRegions[i] = 0;
		}
	}
	m_provRegions.clear();

	for (int i = m_blockRegion.size() - 1;i >= 0;i--)
	{
		if (m_blockRegion[i])
		{
			m_blockRegion[i]->Shutdown();
			::operator delete(m_blockRegion[i], sizeof(*m_blockRegion[i]), 2);
			m_blockRegion[i] = 0;
		}
	}
	m_blockRegion.clear();

	for (int i = m_nationRegion.size() - 1;i >= 0;i--)
	{
		if (m_nationRegion[i])
		{
			m_nationRegion[i]->Shutdown();
			::operator delete(m_nationRegion[i], sizeof(*m_nationRegion[i]), 2);
			m_nationRegion[i] = 0;
		}
	}
	m_nationRegion.clear();

	if (m_instance)
	{
		::operator delete(m_instance, sizeof(*m_instance), 1);
		m_instance = 0;
	}
}

ProvRegionClass * ProvRegionManagerClass::getProvRegion(GlobalManagerClass::regionType type, int provRegionID)
{
	switch (type)
	{
	case GlobalManagerClass::BASE:
		if (m_provRegions.size() > provRegionID)
			return m_provRegions[provRegionID];
		else
			return 0;
	case GlobalManagerClass::BLOCK:
		if (m_blockRegion.size() > provRegionID)
			return m_blockRegion[provRegionID];
		else
			return 0;
	case GlobalManagerClass::NATION:
		if (m_nationRegion.size() > provRegionID)
			return m_nationRegion[provRegionID];
		else
			return 0;
	default:
		return 0;
	}
}

void ProvRegionManagerClass::addProvRegion(GlobalManagerClass::regionType type, ProvRegionClass* region)
{
	switch (type)
	{
	case GlobalManagerClass::BASE:
		m_provRegions.emplace_back(region);
		break;
	case GlobalManagerClass::BLOCK:
		m_blockRegion.emplace_back((BlockRegionClass*)region);
		break;
	case GlobalManagerClass::NATION:
		m_nationRegion.emplace_back((NationRegionClass*)region);
		break;
	default:
		break;
	}
}

ProvRegionManagerClass & ProvRegionManagerClass::getI()
{
	if (!m_instance)
		m_instance = new(1) ProvRegionManagerClass;
	else
		return *m_instance;
}
