////////////////////////////////////////////////////////////////////////////////
// Filename: MeshManagerClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _MODELSARRAYCLASS_H_
#define _MODELSARRAYCLASS_H_

//////////////
// INCLUDES //
//////////////


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "meshclass.h"
#include "ModManagerClass.h"


using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelsArrayClass
////////////////////////////////////////////////////////////////////////////////

class ModelsArrayClass
{
public:

	ModelsArrayClass();
	ModelsArrayClass(const ModelsArrayClass&);
	~ModelsArrayClass();

	void Shutdown();

	bool addModel(ID3D10Device* device, string filename);
	MeshClass* getModel(string filename);
	void setModChanges(map<string, string>* modChanges);

private:
	map<string, string>* m_modChanges;
	map<string, MeshClass*> m_models;
	string getPrefix(string filename);
};

#endif