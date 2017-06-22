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

	bool addModel(ID3D11Device* device, const std::string& filename);
	MeshClass* getModel(const std::string& filename);

	static MeshManagerClass& getI();

private:
	MeshManagerClass();
	MeshManagerClass(const MeshManagerClass&);
	~MeshManagerClass();
private:
	std::map<long long, MeshClass*> m_models;

	static MeshManagerClass* m_instance;
};

#endif