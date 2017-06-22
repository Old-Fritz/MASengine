#include "ResourceManagerClass.h"

ResourceManagerClass* ResourceManagerClass::m_instance = 0;

ResourceManagerClass::ResourceManagerClass()
{
}
ResourceManagerClass::ResourceManagerClass(const ResourceManagerClass &)
{
}
ResourceManagerClass::~ResourceManagerClass()
{
}

bool ResourceManagerClass::Initialize()
{
	//create all blocks

	if (!&TextureManagerClass::getI())
		return false;

	if (!&TextManagerClass::getI())
		return false;

	if (!&MeshManagerClass::getI())
		return false;

	if (!&FontManagerClass::getI())
		return false;

	return true;
}

void ResourceManagerClass::Shutdown()
{
	//Shutdown all blocks
	TextureManagerClass::getI().Shutdown();
	TextManagerClass::getI().Shutdown();
	MeshManagerClass::getI().Shutdown();
	FontManagerClass::getI().Shutdown();

	if (m_instance)
	{
		::operator delete(m_instance, sizeof(*m_instance), 1);
		m_instance = 0;
	}

	return;
}

ResourceManagerClass & ResourceManagerClass::getI()
{
	if (!m_instance)
		m_instance = new(1) ResourceManagerClass;
	return *m_instance;
}