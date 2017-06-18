#include "MeshManagerClass.h"

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
		MemoryManagerClass::getI().deletePool(m_models.begin()->second, sizeof(m_models.begin()->second));
		m_models.erase(m_models.begin());
	}
}

bool MeshManagerClass::addModel(ID3D10Device * device, const std::string & filename)
{
	bool result;

	//create new model
	MeshClass* newModel = (MeshClass*)MemoryManagerClass::getI().getPoolMemory(sizeof(MeshClass));
	if (!newModel)
		return false;

	result = newModel->Initialize(device, filename);
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 4-1");
		return false;
	}

	m_models.emplace(std::pair<long long, MeshClass*>(ModManager.getHash(filename),newModel));

	return true;
}

MeshClass * MeshManagerClass::getModel(const std::string & filename)
{
	return m_models.find(ModManager.getHash(filename))->second;
}
