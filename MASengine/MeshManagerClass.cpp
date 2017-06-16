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
		MemoryManager.deletePool(m_models.begin()->second, sizeof(m_models.begin()->second));
		m_models.erase(m_models.begin());
	}
}

bool MeshManagerClass::addModel(ID3D10Device * device, const std::string & filename)
{
	return false;
}

MeshClass * MeshManagerClass::getModel(const std::string & filename)
{
	return nullptr;
}
