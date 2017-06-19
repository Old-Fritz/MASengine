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
	if (!m_textures)
		return false;

	m_texts = new(1) TextManagerClass;
	if (!m_texts)
		return false;

	m_models = new(1) MeshManagerClass;
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
		::operator delete(m_models, sizeof(MeshManagerClass), 1);
		m_models = 0;
	}
	if (m_texts)
	{
		m_texts->Shutdown();
		::operator delete(m_texts, sizeof(TextManagerClass), 1);
		m_texts = 0;
	}
	if (m_textures)
	{
		m_textures->Shutdown();
		::operator delete(m_textures, sizeof(TextureManagerClass), 1);
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

