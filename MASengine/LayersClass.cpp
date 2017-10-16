#include "LayersClass.h"

LayersClass::LayersClass()
{
	m_baseColor = D3DXVECTOR4(0, 0, 0, 1);
	m_blockColor = D3DXVECTOR4(0, 0, 0, 1);
	m_nationColor = D3DXVECTOR4(0, 0, 0, 1);
	m_mainColor = &m_baseColor;
}
LayersClass::LayersClass(const LayersClass &)
{
}
LayersClass::~LayersClass()
{
}

void LayersClass::setMainColor(GlobalManagerClass::regionType type)
{
	switch (type)
	{
	case GlobalManagerClass::BASE:
		m_mainColor = &m_baseColor;
		break;
	case GlobalManagerClass::BLOCK:
		m_mainColor = &m_blockColor;
		break;
	case GlobalManagerClass::NATION:
		m_mainColor = &m_nationColor;
		break;
	default:
		break;
	}
}
void LayersClass::setColor(GlobalManagerClass::regionType type, D3DXVECTOR4 color)
{
	switch (type)
	{
	case GlobalManagerClass::BASE:
		m_baseColor = color;
		break;
	case GlobalManagerClass::BLOCK:
		m_blockColor = color;
		break;
	case GlobalManagerClass::NATION:
		m_nationColor = color;
		break;
	default:
		break;
	}
}
D3DXVECTOR4 LayersClass::getMainColor()
{
	return *m_mainColor;
}

D3DXVECTOR4 LayersClass::getColor(GlobalManagerClass::regionType type)
{
	switch (type)
	{
	case GlobalManagerClass::BASE:
		return m_baseColor;
	case GlobalManagerClass::BLOCK:
		return m_blockColor;
	case GlobalManagerClass::NATION:
		return m_nationColor;
	default:
		return *m_mainColor;
	}
}