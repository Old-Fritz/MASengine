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
	void addRegion(GlobalManagerClass::regionType type, int regionID);
	void deleteRegion(GlobalManagerClass::regionType type, int regionID);
	std::set<int> getRegions(GlobalManagerClass::regionType type);
private:
	//void readFromFile(string filename, FractionManagerClass* fractions);
	bool readFromFile(std::ifstream* file);
private:
	int m_id;
	std::string m_name;

	LayersClass* m_layers;

	D3DXVECTOR3 m_coords;

	//Prov Regions
	std::set<int> m_baseRegion;
	std::set<int> m_blockRegion;
	std::set<int> m_nationRegion;
};

#endif