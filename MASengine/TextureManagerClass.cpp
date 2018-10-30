#include "TextureManagerClass.h"

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
	// delete all textures
	while (!m_textures.empty())
	{
		m_textures.begin()->second->Shutdown(); //delete texture
		::operator delete(m_textures.begin()->second, sizeof(TextureClass), 2);
		m_textures.erase(m_textures.begin());
	}
}

bool TextureManagerClass::Initialize(ID3D11Device * device)
{
	return false;
}

bool TextureManagerClass::addTexture(ID3D11Device * device, PathClass* filename)
{
	bool result;

	//check for existing
	auto texture = m_textures.find(filename->getHash());
	if (texture == m_textures.end())
	{
		//create new texture
		TextureClass* newTexture = new(4) TextureClass;
		if (!newTexture)
			return false;

		result = newTexture->Initialize(device, filename->getWPath().c_str());
		if (!result)
		{
			GM::LM()->addLog("Error 4-2");
			return false;
		}

		m_textures.emplace(std::pair<int, TextureClass*>(filename->getHash(), newTexture));
	}

	
	return true;
}

void TextureManagerClass::deleteTexture(PathClass* filename)
{
	//delete if it exists
	auto texture = m_textures.find(filename->getHash());
	if (texture != m_textures.end())
	{
		texture->second->Shutdown();
		::operator delete(texture->second, sizeof(*(texture->second)), 2);
		texture->second = 0;
		m_textures.erase(texture);
	}

}

void TextureManagerClass::deleteTexture(int hash)
{
}

TextureClass * TextureManagerClass::getTexture(ID3D11Device* device, PathClass* filename)
{
	return getTexture(filename->getHash());
}

TextureClass * TextureManagerClass::getTexture(int hash)
{
	auto texture = m_textures.find(hash);
	if (texture != m_textures.end())
	{
		return texture->second;
	}
	else
		return NULL;
}
