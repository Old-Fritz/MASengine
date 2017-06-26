#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_shaderManager = 0;
	m_camera = 0;
	m_test = 0;
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
	LogManagerClass::getI().addLog("Directx Initialization");

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
	LogManagerClass::getI().addLog("Shaders Initialization");

	//initialize Camera
	m_camera = new(1) CameraClass;
	if (!m_camera)
		return false;

	// Set the initial position of the camera.
	m_camera->SetPosition(D3DXVECTOR3(0,0,-10));

	//Init test
	m_test = new InterfaceElementClass;
	if (!result)
		return false;
	result = m_test->Initialize(m_D3D->GetDevice(),m_D3D->GetDeviceContext(), hwnd, "data/interface/testEl.txt", SettingsClass::getI().getIntParameter("ScreenWidth"), SettingsClass::getI().getIntParameter("ScreenHeight"));
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize test", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	if (m_test)
	{
		m_test->Shutdown();
		::operator delete(m_test, sizeof(TextClass), 1);
		m_test = 0;
	}

	if (m_camera)
	{
		::operator delete(m_camera, sizeof(CameraClass), 1);
		m_camera = 0;
	}

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

	result = m_test->updateTSadding(m_D3D->GetDeviceContext(), "maga", 0, std::to_string(SystemStateManagerClass::getI().GetFps()));
	if(!result)
	{
		return false;
	}
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
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));

	// Generate the view matrix based on the camera's position.
	m_camera->Render();

	// Get the world, view, projection, and ortho matrices from the camera and d3d objects.
	m_camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	m_D3D->TurnZBufferOff();

	m_test->Render(m_shaderManager->getFontShader(), m_shaderManager->getInterfaceShader(), m_D3D->GetDeviceContext(), worldMatrix,orthoMatrix, viewMatrix);

	m_D3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}
