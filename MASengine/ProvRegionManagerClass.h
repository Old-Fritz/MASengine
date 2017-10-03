////////////////////////////////////////////////////////////////////////////////
// Filename: ProvRegionManagerClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _PROVREGIONMANAGERCLASS_H_
#define _PROVREGIONMANAGERCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "provManagerClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ProvRegionManagerClass
////////////////////////////////////////////////////////////////////////////////

class ProvRegionManagerClass
{

public:
	

	bool Initialize(PathClass* filename);
	void Shutdown();

	//Getters
	ProvRegionClass* getProvRegion(int provRegionID);

	static ProvRegionManagerClass& getI();
private:
	ProvRegionManagerClass();
	ProvRegionManagerClass(const ProvRegionManagerClass&);
	~ProvRegionManagerClass();
private:
	static ProvRegionManagerClass* m_instance;

	std::vector<ProvRegionClass*> m_provRegions;
};

#endif