#include "BlockRegionClass.h"

void BlockRegionClass::add(int provID)
{
	ProvRegionClass::add(provID);

	ProvClass* prov = ProvManagerClass::getI().getProv(provID);

	prov->addRegion(GM::BLOCK, m_id);
	prov->getLayers()->setColor(GM::BLOCK, m_color);
	
}
