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
	
	//open file with base regions
	std::ifstream file;
	file.open(filename->getPath());
	//stop if file doesn`t exist
	if (!file.is_open())
		return false;

	//initialize base regions for all types
	//init base region
	ProvRegionClass* baseRegion = new(4) ProvRegionClass;
	if (!baseRegion)
		return false;
	result = baseRegion->Initialize(&file,0);
	if (!result)
		return false;

	//init block region
	BlockRegionClass* blockRegion = new(4) BlockRegionClass;
	if (!blockRegion)
		return false;
	result = blockRegion->Initialize(&file, 0);
	if (!result)
		return false;

	file.close();

	//init nation region
	NationRegionClass* nationRegion = new(4) NationRegionClass;
	if (!nationRegion)
		return false;
	result = nationRegion->Initialize(&file, 0);
	if (!result)
		return false;

	//add all provs to base blocks
	/*
	for (int i = 0;i < ProvManagerClass::getI().getProvNum(); i++)
	{
		baseRegion->add(i);
		nationRegion->add(i);
		blockRegion->add(i);
	}
	*/
	//add regions to vectots
	m_provRegions.emplace_back(baseRegion);
	m_blockRegion.emplace_back(blockRegion);
	m_nationRegion.emplace_back(nationRegion);

	
	return true;

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
