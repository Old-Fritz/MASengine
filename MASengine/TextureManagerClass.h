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

	TextureManagerClass();
	TextureManagerClass(const TextureManagerClass&);
	~TextureManagerClass();

	void Shutdown();

	bool addTexture(ID3D10Device* device, const std::string& filename);
	ID3D10ShaderResourceView* getTexture(const std::string& filename);

private:
	std::map<long long, TextureClass*> m_textures;
	
};

#endif