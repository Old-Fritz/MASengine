#include "ShaderManagerClass.h"

ShaderManagerClass::ShaderManagerClass()
{
	m_interfaceShader = 0;
	m_fontShader = 0;
	m_terrainShader = 0;
}
ShaderManagerClass::ShaderManagerClass(const ShaderManagerClass &)
{
}
ShaderManagerClass::~ShaderManagerClass()
{
}

bool ShaderManagerClass::Initialize(ID3D11Device * device, HWND hwnd)
{
	bool result;

	//Initialize interface Shader
	m_interfaceShader = new(1) InterfaceShaderClass;
	if (!m_interfaceShader)
		return false;
	PathClass* interfacePS = PathManagerClass::getI().makePath("interfacePS.fx");
	PathClass* interfaceVS = PathManagerClass::getI().makePath("interfaceVS.fx");
	result = m_interfaceShader->Initialize(device, hwnd, interfaceVS, interfacePS);
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 10-1");
		return false;
	}
	LogManagerClass::getI().addLog("Interface Shader Initialization");

	//Initialize font Shader
	m_fontShader = new(1) FontShaderClass;
	if (!m_fontShader)
		return false;
	PathClass* fontPS = PathManagerClass::getI().makePath("fontPS.fx");
	PathClass* fontVS = PathManagerClass::getI().makePath("interfaceVS.fx");
	result = m_fontShader->Initialize(device, hwnd, fontVS, fontPS);
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 10-3");
		return false;
	}
	LogManagerClass::getI().addLog("Font Shader Initialization");

	//Initialize terrain Shader
	m_terrainShader = new(1) TerrainShaderClass;
	if (!m_terrainShader)
		return false;
	PathClass* terrainPS = PathManagerClass::getI().makePath("terrainPS.fx");
	PathClass* terrainVS = PathManagerClass::getI().makePath("terrainVS.fx");
	result = m_terrainShader->Initialize(device, hwnd, terrainVS, terrainPS);
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 10-x");
		return false;
	}
	LogManagerClass::getI().addLog("Terrain Shader Initialization");

	return true;
}

void ShaderManagerClass::Shutdown()
{
	if (m_terrainShader)
	{
		m_terrainShader->Shutdown();
		::operator delete(m_terrainShader, sizeof(*m_terrainShader), 1);
		m_terrainShader = 0;
	}

	if (m_fontShader)
	{
		m_fontShader->Shutdown();
		::operator delete(m_fontShader, sizeof(FontShaderClass), 1);
		m_fontShader = 0;
	}

	if (m_interfaceShader)
	{
		m_interfaceShader->Shutdown();
		::operator delete(m_interfaceShader, sizeof(InterfaceShaderClass), 1);
		m_interfaceShader = 0;
	}

	return;
}

InterfaceShaderClass * ShaderManagerClass::getInterfaceShader()
{
	return m_interfaceShader;
}

FontShaderClass * ShaderManagerClass::getFontShader()
{
	return m_fontShader;
}

TerrainShaderClass * ShaderManagerClass::getTerrainShader()
{
	return m_terrainShader;
}
