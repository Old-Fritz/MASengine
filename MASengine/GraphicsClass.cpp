#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_shaderManager = 0;
}

GraphicsClass::GraphicsClass(const GraphicsClass &)
{
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(HWND hwnd)
{
	bool result;


	// Create the Direct3D object.
	m_D3D = new(1) D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(hwnd, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 9-17");
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	//Initialize Shader Manager
	m_shaderManager = new(1) ShaderManagerClass;
	if (!m_shaderManager)
		return false;
	result = m_shaderManager->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Shaders", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	if (m_shaderManager)
	{
		m_shaderManager->Shutdown();
		::operator delete(m_shaderManager, sizeof(ShaderManagerClass), 1);
		m_shaderManager = 0;
	}
	if (m_D3D)
	{
		m_D3D->Shutdown();
		::operator delete( m_D3D,sizeof(D3DClass),1);
		m_D3D = 0;
	}
}

bool GraphicsClass::Frame()
{
	bool result;


	// Render the graphics scene.
	result = Render();
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 8-3");
		return false;
	}

	return true;
}

bool GraphicsClass::Render()
{
	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));


	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}
