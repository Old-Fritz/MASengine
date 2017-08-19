////////////////////////////////////////////////////////////////////////////////
// Filename: ProvRegionClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _PROVREGIONCLASS_H_
#define _PROVREGIONCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "GlobalManagerClass.h"
#include <set>

////////////////////////////////////////////////////////////////////////////////
// Class name: ProvClass
////////////////////////////////////////////////////////////////////////////////

class ProvRegionClass
{

public:
	ProvRegionClass();
	ProvRegionClass(const ProvRegionClass&);
	~ProvRegionClass();

	bool Initialize(std::ifstream* file, int id);
	void Shutdown();

	void add(int provID);
	void erase(int provID);
	

	//Getters
	int getID();
	std::set<int>* getProvIDs();
private:
	//void readFromFile(string filename, FractionManagerClass* fractions);
	bool readFromFile(std::ifstream* file);
private:
	int m_id;
	
	D3DXVECTOR4 m_color = D3DXVECTOR4(0,0,0,1);

	std::set<int> m_provIDs;
};

#endif