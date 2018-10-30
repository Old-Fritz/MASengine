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

void LayersClass::setMainColor(GM::regionType type)
{
	switch (type)
	{
	case GM::BASE:
		m_mainColor = &m_baseColor;
		break;
	case GM::BLOCK:
		m_mainColor = &m_blockColor;
		break;
	case GM::NATION:
		m_mainColor = &m_nationColor;
		break;
	default:
		break;
	}
}
void LayersClass::setColor(GM::regionType type, D3DXVECTOR4 color)
{
	switch (type)
	{
	case GM::BASE:
		m_baseColor = color;
		break;
	case GM::BLOCK:
		m_blockColor = color;
		break;
	case GM::NATION:
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

D3DXVECTOR4 LayersClass::getColor(GM::regionType type)
{
	switch (type)
	{
	case GM::BASE:
		return m_baseColor;
	case GM::BLOCK:
		return m_blockColor;
	case GM::NATION:
		return m_nationColor;
	default:
		return *m_mainColor;
	}
}