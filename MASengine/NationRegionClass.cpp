#include "NationRegionClass.h"

void NationRegionClass::add(int provID)
{
	ProvRegionClass::add(provID);

	ProvClass* prov = ProvManagerClass::getI().getProv(provID);
	//check if prov has this type of region
	auto regions = prov->getRegions(GM::NATION);
	//if (regions.size() > 0)
	//delete this element if it exist to push it to top
	prov->deleteRegion(GM::NATION, m_id);
	//add new region
	prov->addRegion(GM::NATION, m_id);
	//change color
	prov->getLayers()->setColor(GM::NATION, m_color);

}
