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
#include "PathManagerClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: TextureManagerClass
////////////////////////////////////////////////////////////////////////////////

class TextureManagerClass
{
public:
	void Shutdown();

	static TextureManagerClass& getI();

	bool addTexture(ID3D11Device* device, PathClass* filename);
	void deleteTexture(PathClass* filename);
	ID3D11ShaderResourceView* getTexture(PathClass* filename);
	ID3D11ShaderResourceView* getTexture(int hash);
	ID3D11ShaderResourceView** getTexturesArray(int* hashes, int num);
private:
	TextureManagerClass();
	TextureManagerClass(const TextureManagerClass&);
	~TextureManagerClass();
private:
	std::map<int, TextureClass*> m_textures;

	static TextureManagerClass* m_instance;
	
};

#endif