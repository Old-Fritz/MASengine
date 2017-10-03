////////////////////////////////////////////////////////////////////////////////
// Filename: MeshManagerClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _MESHMANAGERCLASS_H_
#define _MESHMANAGERCLASS_H_

//////////////
// INCLUDES //
//////////////


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "meshclass.h"
#include "ModManagerClass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: MeshManagerClass
////////////////////////////////////////////////////////////////////////////////

class MeshManagerClass
{
public:
	void Shutdown();

	bool addModel(ID3D11Device* device, PathClass* filename, float sizeX = 0, float sizeY = 0, float sizeZ = 0);
	MeshClass* getModel(PathClass* filename);
	MeshClass* getModel(int hash);

	static MeshManagerClass& getI();

private:
	MeshManagerClass();
	MeshManagerClass(const MeshManagerClass&);
	~MeshManagerClass();
private:
	std::map<int, MeshClass*> m_models;

	static MeshManagerClass* m_instance;
};

#endif