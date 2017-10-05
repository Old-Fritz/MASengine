#include "LightClass.h"

LightClass::LightClass()
{
}
LightClass::LightClass(const LightClass &)
{
}
LightClass::~LightClass()
{
}

void LightClass::SetAmbientColor(D3DXVECTOR4 color)
{
	m_ambientColor = color;
}

void LightClass::SetDiffuseColor(D3DXVECTOR4 color)
{
	m_diffuseColor = color;
}

void LightClass::SetDirection(D3DXVECTOR3 direction)
{
	m_direction = direction;
}

void LightClass::SetSpecularColor(D3DXVECTOR4 color)
{
	m_specularColor = color;
}

void LightClass::SetSpecularPower(float power)
{
	m_specularPower = power;
}

D3DXVECTOR4 LightClass::GetAmbientColor()
{
	return m_ambientColor;
}

D3DXVECTOR4 LightClass::GetDiffuseColor()
{
	return m_diffuseColor;
}

D3DXVECTOR3 LightClass::GetDirection()
{
	return m_direction;
}

D3DXVECTOR4 LightClass::GetSpecularColor()
{
	return m_specularColor;
}

float LightClass::GetSpecularPower()
{
	return m_specularPower;
}
