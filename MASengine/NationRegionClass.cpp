#include "NationRegionClass.h"

void NationRegionClass::add(int provID)
{
	ProvRegionClass::add(provID);

	ProvClass* prov = ProvManagerClass::getI().getProv(provID);
	//check if prov has this type of region
	auto regions = prov->getRegions(GlobalManagerClass::NATION);
	//if (regions.size() > 0)
	//delete this element if it exist to push it to top
	prov->deleteRegion(GlobalManagerClass::NATION, m_id);
	//add new region
	prov->addRegion(GlobalManagerClass::NATION, m_id);
	//change color
	prov->getLayers()->setColor(GlobalManagerClass::NATION, m_color);

}
