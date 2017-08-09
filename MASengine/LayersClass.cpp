#include "LayersClass.h"

LayersClass::LayersClass()
{
}
LayersClass::LayersClass(const LayersClass &)
{
}
LayersClass::~LayersClass()
{
}

void LayersClass::setMainColor(D3DXVECTOR4 color)
{
	m_mainColor = color;
}
void LayersClass::setBaseColor(D3DXVECTOR4 color)
{
	m_baseColor = color;
}
D3DXVECTOR4 LayersClass::getMainColor()
{
	return m_mainColor;
}

D3DXVECTOR4 LayersClass::getBaseColor()
{
	return m_baseColor;
}
