#include "BlockRegionClass.h"

void BlockRegionClass::add(int provID)
{
	ProvRegionClass::add(provID);

	ProvClass* prov = ProvManagerClass::getI().getProv(provID);

	prov->addRegion(GlobalManagerClass::BLOCK, m_id);
	prov->getLayers()->setColor(GlobalManagerClass::BLOCK, m_color);
	
}
