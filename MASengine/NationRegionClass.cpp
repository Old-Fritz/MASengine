#include "NationRegionClass.h"

void NationRegionClass::add(int provID)
{
	ProvClass* prov = ProvManagerClass::getI().getProv(provID);
	//check if prov has this type of region
	auto regions = prov->getRegions(GlobalManagerClass::NATION);
	if (regions.size() > 0)
		//delete last element
		prov->deleteRegion(GlobalManagerClass::NATION, *regions.end());
	//add new region
	prov->addRegion(GlobalManagerClass::NATION, m_id);
	//change color
	prov->getLayers()->setColor(GlobalManagerClass::NATION, m_color);

}
