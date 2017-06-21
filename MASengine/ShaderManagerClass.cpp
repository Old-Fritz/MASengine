#include "ShaderManagerClass.h"

ShaderManagerClass::ShaderManagerClass()
{
	m_interfaceShader = 0;
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

	return true;
}

void ShaderManagerClass::Shutdown()
{
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
