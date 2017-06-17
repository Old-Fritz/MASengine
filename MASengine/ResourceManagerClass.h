
////////////////////////////////////////////////////////////////////////////////
// Filename: ResourceManagerClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _RESOURCEMANAGERCLASS_H_
#define _RESOURCEMANAGERCLASS_H_

//////////////
// INCLUDES //
//////////////


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "TextureManagerClass.h"
#include "TextManagerClass.h"
#include "MeshManagerClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ResourceManagerClass
////////////////////////////////////////////////////////////////////////////////

class ResourceManagerClass
{
public:

	ResourceManagerClass();
	ResourceManagerClass(const ResourceManagerClass&);
	~ResourceManagerClass();

	bool Initialize();
	void Shutdown();

	//get modules
	TextureManagerClass* getTextures();
	TextManagerClass* getTexts();
	MeshManagerClass* getModels();
private:
	//modules
	TextureManagerClass* m_textures;
	TextManagerClass* m_texts;
	MeshManagerClass* m_models;

	float m_frametime;
};

/////////////
// GLOBALS //
/////////////
static ResourceManagerClass ResourceManager;

#endif
