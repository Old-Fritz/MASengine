#include "ShaderManagerClass.h"

ShaderManagerClass::ShaderManagerClass()
{
	m_interfaceShader = 0;
	m_fontShader = 0;
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
	result = m_interfaceShader->Initialize(device, hwnd);
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
	result = m_fontShader->Initialize(device, hwnd);
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 10-3");
		return false;
	}
	LogManagerClass::getI().addLog("Font Shader Initialization");

	return true;
}

void ShaderManagerClass::Shutdown()
{
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
