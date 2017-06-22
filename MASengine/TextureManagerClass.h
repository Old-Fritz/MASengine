////////////////////////////////////////////////////////////////////////////////
// Filename: TextureManagerClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _TEXTUREMANAGERCLASS_H_
#define _TEXTUREMANAGERCLASS_H_

//////////////
// INCLUDES //
//////////////


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "TextureClass.h"
#include "ModManagerClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: TextureManagerClass
////////////////////////////////////////////////////////////////////////////////

class TextureManagerClass
{
public:
	void Shutdown();

	static TextureManagerClass& getI();

	bool addTexture(ID3D11Device* device, const std::string& filename);
	ID3D11ShaderResourceView* getTexture(const std::string& filename);

private:
	TextureManagerClass();
	TextureManagerClass(const TextureManagerClass&);
	~TextureManagerClass();
private:
	std::map<long long, TextureClass*> m_textures;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> m_converter;

	static TextureManagerClass* m_instance;
	
};

#endif