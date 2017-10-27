#include "ProvManagerClass.h"

ProvManagerClass* ProvManagerClass::m_instance = 0;

bool ProvManagerClass::Initialize(PathClass* filename)
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
		ProvClass* prov = new(1) ProvClass;
		result = prov->Initialize(&file, i);
		if (!result)
		{
			return false;
		}
		if(i%100==0)
			LoadScreenManagerClass::getI().changeLine("Init prov" + std::to_string(i), 0.2f + 0.2f * ((float)i / (float)numOfProvs));
		m_provs.emplace_back(prov);
	}

	return true;
}

void ProvManagerClass::Shutdown()
{
	for (int i = m_provs.size() - 1;i>=0;i--)
	{
		if (m_provs[i])
		{
			m_provs[i]->Shutdown();
			::operator delete(m_provs[i], sizeof(*m_provs[i]), 1);
			m_provs[i] = 0;
		}
	}
	m_provs.clear();

	if (m_instance)
	{
		::operator delete(m_instance, sizeof(*m_instance), 1);
		m_instance = 0;
	}
}

ProvClass * ProvManagerClass::getProv(int provNum)
{
	if (provNum < m_provs.size())
		return m_provs[provNum];
	else
		return m_provs[0];
}

void ProvManagerClass::setGlobalMainColor(GlobalManagerClass::regionType type)
{
	for (int i = 0;i < m_provs.size();i++)
		m_provs[i]->getLayers()->setMainColor(type);
}

int ProvManagerClass::getProvNum()
{
	return m_provs.size();
}

ProvManagerClass & ProvManagerClass::getI()
{
	if (!m_instance)
		m_instance = new(1) ProvManagerClass;
	else
		return *m_instance;
}

ProvManagerClass::ProvManagerClass()
{
}
ProvManagerClass::~ProvManagerClass()
{
}
