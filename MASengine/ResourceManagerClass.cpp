#include "ResourceManagerClass.h"

ResourceManagerClass::ResourceManagerClass()
{
	m_textures = 0;
	m_models = 0;
	m_texts = 0;
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

	m_textures = new(1) TextureManagerClass;
	//m_textures = static_cast<TextureManagerClass*>(MemoryManagerClass::getI().getStackMemory(sizeof(TextureManagerClass)));
	if (!m_textures)
		return false;

	m_texts = (TextManagerClass*)MemoryManagerClass::getI().getStackMemory(sizeof(TextManagerClass));
	if (!m_texts)
		return false;

	m_models = (MeshManagerClass*)MemoryManagerClass::getI().getStackMemory(sizeof(MeshManagerClass));
	if (!m_models)
		return false;

	return true;
}

void ResourceManagerClass::Shutdown()
{
	//Shutdown all blocks
	if (m_models)
	{
		m_models->Shutdown();
		MemoryManagerClass::getI().deleteStack(m_models, sizeof(m_models));
		m_models = 0;
	}
	if (m_texts)
	{
		m_texts->Shutdown();
		MemoryManagerClass::getI().deleteStack(m_texts, sizeof(m_texts));
		m_texts = 0;
	}
	if (m_textures)
	{
		m_textures->Shutdown();
		MemoryManagerClass::getI().deleteStack(m_textures, sizeof(m_textures));
		m_textures = 0;
	}

	return;
}

TextureManagerClass * ResourceManagerClass::getTextures()
{
	return m_textures;
}

TextManagerClass * ResourceManagerClass::getTexts()
{
	return m_texts;
}

MeshManagerClass * ResourceManagerClass::getModels()
{
	return m_models;
}

