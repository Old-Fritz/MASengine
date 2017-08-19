////////////////////////////////////////////////////////////////////////////////
// Filename: provclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _PROVCLASS_H_
#define _PROVCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include"GlobalManagerClass.h"
#include "LayersClass.h"
#include "ProvRegionClass.h"
#include <set>

////////////////////////////////////////////////////////////////////////////////
// Class name: ProvClass
////////////////////////////////////////////////////////////////////////////////

class ProvClass
{

public:
	ProvClass();
	ProvClass(const ProvClass&);
	~ProvClass();

	bool Initialize(std::ifstream* file, int id);
	void Shutdown();

	//void save(string filename);

	//Getters
	int getID();
	std::string getName();
	D3DXVECTOR3 getCoords();
	LayersClass* getLayers();

	//Prov Regions
	void changeBlockRegion(ProvRegionClass* newRegion);
private:
	//void readFromFile(string filename, FractionManagerClass* fractions);
	bool readFromFile(std::ifstream* file);
private:
	int m_id;
	std::string m_name;

	LayersClass* m_layers;

	D3DXVECTOR3 m_coords;

	//Prov Regions
	std::set<ProvRegionClass*> m_blockRegion;
};

#endif