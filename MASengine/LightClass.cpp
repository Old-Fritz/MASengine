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

void LightClass::addPointLight(PointLightType * light)
{
	m_pointLights.emplace_back(light);
}

void LightClass::addDirectedLight(DirectedLightType * light)
{
	m_directedLights.emplace_back(light);
}

std::vector<LightClass::PointLightType*> LightClass::getPointLights()
{
	return m_pointLights;
}

std::vector<LightClass::DirectedLightType*> LightClass::getDirectedLights()
{
	return m_directedLights;
}
