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

bool MeshManagerClass::addModel(ID3D11Device * device, PathClass* filename, float sizeX, float sizeY, float sizeZ)
{
	bool result;

	//check for existing of model
	if (m_models.find(filename->getHash()) != m_models.end())
		return true;

	//create new model
	MeshClass* newModel = new(4) MeshClass;
	if (!newModel)
		return false;

	result = newModel->Initialize(device, filename, sizeX, sizeY, sizeZ);
	if (!result)
	{
		GM::LM()->addLog("Error 4-1");
		return false;
	}

	m_models.emplace(std::pair<int, MeshClass*>(filename->getHash(), newModel));

	return true;
}

MeshClass * MeshManagerClass::getModel(PathClass* filename)
{
	return getModel(filename->getHash());

	
}

MeshClass * MeshManagerClass::getModel(int hash)
{
	auto model = m_models.find(hash);

	return model != m_models.end() ? model->second : NULL;
}

MeshManagerClass & MeshManagerClass::getI()
{
	if (!m_instance)
		m_instance = new(1) MeshManagerClass;
	return *m_instance;
}
