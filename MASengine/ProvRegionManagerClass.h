////////////////////////////////////////////////////////////////////////////////
// Filename: ProvRegionManagerClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _PROVREGIONMANAGERCLASS_H_
#define _PROVREGIONMANAGERCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "provRegionClass.h"
#include "NationRegionClass.h"
#include "BlockRegionClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ProvRegionManagerClass
////////////////////////////////////////////////////////////////////////////////

class ProvRegionManagerClass
{

public:
	
	bool Initialize(PathClass* filename);

	void Shutdown();

	//Getters
	ProvRegionClass* getProvRegion(GlobalManagerClass::regionType type, int provRegionID);
	void addProvRegion(GlobalManagerClass::regionType type, ProvRegionClass* region);

	static ProvRegionManagerClass& getI();
private:
	ProvRegionManagerClass();
	ProvRegionManagerClass(const ProvRegionManagerClass&);
	~ProvRegionManagerClass();
private:
	static ProvRegionManagerClass* m_instance;

	std::vector<ProvRegionClass*> m_provRegions;
	std::vector<BlockRegionClass*> m_blockRegion;
	std::vector<NationRegionClass*> m_nationRegion;
};

#endif