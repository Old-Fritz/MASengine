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
#include "ProvManagerClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ProvClass
////////////////////////////////////////////////////////////////////////////////

class ProvRegionClass
{

public:
	ProvRegionClass();
	ProvRegionClass(const ProvRegionClass&);
	~ProvRegionClass();

	virtual bool Initialize(std::ifstream* file, int id);
	virtual void Shutdown();

	virtual void add(int provID);
	virtual void erase(int provID);
	

	//Getters
	virtual int getID();
	virtual std::vector<ProvClass*>* getProvs();
protected:
	//void readFromFile(string filename, FractionManagerClass* fractions);
	bool readFromFile(std::ifstream* file);
protected:
	int m_id;
	
	D3DXVECTOR4 m_color = D3DXVECTOR4(0,0,0,1);

	std::vector<ProvClass*> m_provs;
};

#endif