#include "TextureManagerClass.h"

TextureManagerClass* TextureManagerClass::m_instance = 0;

TextureManagerClass::TextureManagerClass()
{
}
TextureManagerClass::TextureManagerClass(const TextureManagerClass &)
{
}
TextureManagerClass::~TextureManagerClass()
{
}

void TextureManagerClass::Shutdown()
{
	while (!m_textures.empty())
	{
		m_textures.begin()->second->Shutdown(); //delete texture
		::operator delete(m_textures.begin()->second, sizeof(TextureClass), 2);
		m_textures.erase(m_textures.begin());
	}

	if (m_instance)
	{
		::operator delete(m_instance, sizeof(*m_instance), 1);
		m_instance = 0;
	}
}

TextureManagerClass & TextureManagerClass::getI()
{
	if (!m_instance)
		m_instance = new(1) TextureManagerClass;
	return *m_instance;
}

bool TextureManagerClass::addTexture(ID3D11Device * device, const std::string & filename)
{
	bool result;

	//check for existing
	auto texture = m_textures.find(ModManagerClass::getI().getHash(filename));
	if (texture == m_textures.end())
	{
		//create new texture
		TextureClass* newTexture = new(4) TextureClass;
		if (!newTexture)
			return false;

		result = newTexture->Initialize(device, m_converter.from_bytes(filename).c_str());
		if (!result)
		{
			LogManagerClass::getI().addLog("Error 4-2");
			return false;
		}

		m_textures.emplace(std::pair<long long, TextureClass*>(ModManagerClass::getI().getHash(filename), newTexture));
	}

	
	return true;
}

void TextureManagerClass::deleteTexture(const std::string & filename)
{
	//delete if it exists
	auto texture = m_textures.find(ModManagerClass::getI().getHash(filename));
	if (texture != m_textures.end())
	{
		texture->second->Shutdown();
		::operator delete(texture->second, sizeof(*(texture->second)), 2);
		texture->second = 0;
		m_textures.erase(texture);
	}

}

ID3D11ShaderResourceView * TextureManagerClass::getTexture(const std::string & filename)
{
	auto texture = m_textures.find(ModManagerClass::getI().getHash(filename));
	if (texture != m_textures.end())
	{
		return texture->second->GetTexture();
	}
	else
		return NULL;
	
}
