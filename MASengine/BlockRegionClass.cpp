#include "BlockRegionClass.h"

void BlockRegionClass::add(int provID)
{
	ProvClass* prov = ProvManagerClass::getI().getProv(provID);
	m_provs.insert(prov);

	prov->addRegion(GlobalManagerClass::BLOCK, m_id);
	prov->getLayers()->setColor(GlobalManagerClass::BLOCK, m_color);
	
}
