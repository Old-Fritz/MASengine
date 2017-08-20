#include "MeshManagerClass.h"

MeshManagerClass* MeshManagerClass::m_instance = 0;

MeshManagerClass::MeshManagerClass()
{
}
MeshManagerClass::MeshManagerClass(const MeshManagerClass &)
{
}
MeshManagerClass::~MeshManagerClass()
{
}

void MeshManagerClass::Shutdown()
{
	while (!m_models.empty())
	{
		m_models.begin()->second->Shutdown(); //delete all commands
		::operator delete(m_models.begin()->second, sizeof(MeshClass),2);
		m_models.erase(m_models.begin());
	}

	if (m_instance)
	{
		::operator delete(m_instance, sizeof(*m_instance), 1);
		m_instance = 0;
	}
}

bool MeshManagerClass::addModel(ID3D11Device * device, const std::string & filename)
{
	bool result;

	//create new model
	MeshClass* newModel = new MeshClass;
	if (!newModel)
		return false;

	result = newModel->Initialize(device, filename);
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 4-1");
		return false;
	}

	m_models.emplace(std::pair<long long, MeshClass*>(ModManagerClass::getI().getHash(filename),newModel));

	return true;
}

bool MeshManagerClass::addModel(ID3D11Device * device, const std::string & filename, int lvl)
{
	if(!lvl)
		return addModel(device,filename);
	else
	{
		return false;
	}
}

MeshClass * MeshManagerClass::getModel(const std::string & filename)
{
	return m_models.find(ModManagerClass::getI().getHash(filename))->second;
}

MeshClass * MeshManagerClass::getModel(int hash)
{
	return m_models.find(hash)->second;
}

MeshManagerClass & MeshManagerClass::getI()
{
	if (!m_instance)
		m_instance = new(1) MeshManagerClass;
	return *m_instance;
}
