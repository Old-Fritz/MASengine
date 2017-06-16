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
	while (!m_textures.empty())
	{
		m_textures.begin()->second->Shutdown(); //delete texture
		MemoryManager.deletePool(m_textures.begin()->second, sizeof(m_textures.begin()->second));
		m_textures.erase(m_textures.begin());
	}
}

bool TextureManagerClass::addTexture(ID3D10Device * device, const std::string & filename)
{
	bool result;

	//create new texture
	TextureClass* newTexture = (TextureClass*)MemoryManager.getPoolMemory(sizeof(TextureClass));
	if (!newTexture)
		return false;

	result = newTexture->Initialize(device, filename);
	if (!result)
	{
		LogManager.addLog("Error 4-2");
		return false;
	}

	m_textures.emplace(std::pair<long long, TextureClass*>(ModManager.getHash(filename), newTexture));

	return true;
}

ID3D10ShaderResourceView * TextureManagerClass::getTexture(const std::string & filename)
{
	return m_textures.find(ModManager.getHash(filename))->second->GetTexture();
}