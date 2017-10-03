#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
{

	m_D3D = 0;
	m_shaderManager = 0;
	m_camera = 0;
	m_interface = 0;
	for (int i = 0;i < TEST_NUM;i++)
		m_test[i] = 0;
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

	m_hwnd = hwnd;

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
	m_camera->SetPosition(D3DXVECTOR3(0,0,-1));
	m_camera->Render();
	m_camera->GetViewMatrix(m_baseViewMatrix);

	//Init loadscreen manager
	result = LoadScreenManagerClass::getI().Initialize(m_D3D,m_shaderManager,m_baseViewMatrix,m_hwnd,SettingsClass::getI().getPathParameter("loadScreenManagerFilename"));
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 9-20");
		return false;
	}
	LoadScreenManagerClass::getI().showElements();

	//Init Interface
	m_interface = new(1) InterfaceClass;
	if (!m_interface)
		return false;
	result = m_interface->Initialize(m_D3D->GetDevice(),m_D3D->GetDeviceContext(), hwnd, SettingsClass::getI().getIntParameter("ScreenWidth"), SettingsClass::getI().getIntParameter("ScreenHeight"));
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize interface", L"Error", MB_OK);
		return false;
	}


	for (int i = 0;i < TEST_NUM;i++)
	{
		m_test[i] = new(1) TerrainClass;
		if (!m_test[i])
			return false;
		result = m_test[i]->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), PathManagerClass::getI().makePath("data/terrain/block1.txt"), 0);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize test", L"Error", MB_OK);
			return false;
		}
	}

	

	return true;
}

void GraphicsClass::Shutdown()
{
	for (int i = 0;i < TEST_NUM;i++)
	{
		if (m_test[i])
		{
			m_test[i]->Shutdown();
			::operator delete(m_test[i], sizeof(*m_test[i]), 1);
			m_test[i] = 0;
		}
	}
	

	if (m_interface)
	{
		m_interface->Shutdown();
		::operator delete(m_interface, sizeof(InterfaceClass), 1);
		m_interface = 0;
	}

	LoadScreenManagerClass::getI().Shutdown();

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

bool GraphicsClass::Frame(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int mouseX, int mouseY)
{
	bool result;

	result = m_interface->Frame(m_D3D->GetDeviceContext(),mouseX,mouseY);
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

	//update camera
	m_camera->SetPosition(pos);
	m_camera->SetRotation(rot);
	
	//updating denug info
	m_interface->UpdateDebug(m_D3D->GetDeviceContext(), pos, rot, mouseX, mouseY);

	return true;
}

bool GraphicsClass::updateInterface(CommandClass * command, int ind)
{
	if (command->getCommandsNum() < 1)
		return false;
	std::string updCommandType = command->getParam(ind, 1); // type of update is second param

													 //Updating params of element
	if (updCommandType == "setElVisible")
	{
		m_interface->setElVisible(command->getParam(ind, 2), stoi(command->getParam(ind, 3))); // all params in order
	}
	if (updCommandType == "startDrag")
	{
		m_interface->startDrag(command->getParam(ind, 2)); // all params in order
	}
	if (updCommandType == "endDrag")
	{
		m_interface->endDrag(command->getParam(ind, 2)); // all params in order
	}
	if (updCommandType == "elPos")
	{
		m_interface->setPos(m_D3D->GetDeviceContext(),command->getParam(ind, 2), stoi(command->getParam(ind, 3)), stoi(command->getParam(ind, 4))); // all params in order
	}


	//Updating params of bitmaps
	else if (updCommandType == "BMposX")
	{
		if (command->getParamsNum(ind) >= 5) //if number of params smaller than normal, then this is incorrect
			m_interface->updateElBMposX(command->getParam(ind, 2), command->getParam(ind, 3), stoi(command->getParam(ind, 4))); // all params in order
	}
	else if (updCommandType == "BMposY")
	{
		if (command->getParamsNum(ind) >= 5) //if number of params smaller than normal, then this is incorrect
			m_interface->updateElBMposY(command->getParam(ind, 2), command->getParam(ind, 3), stoi(command->getParam(ind, 4))); // all params in order
	}
	else if (updCommandType == "BMvertPercent")
	{
		if (command->getParamsNum(ind) >= 5) //if number of params smaller than normal, then this is incorrect
			m_interface->updateElBMvertPercent(command->getParam(ind, 2), command->getParam(ind, 3), stof(command->getParam(ind, 4))); // all params in order
	}
	else if (updCommandType == "BMhorPercent")
	{
		if (command->getParamsNum(ind) >= 5) //if number of params smaller than normal, then this is incorrect
			m_interface->updateElBMhorPercent(command->getParam(ind, 2), command->getParam(ind, 3), stof(command->getParam(ind, 4))); // all params in order
	}
	else if (updCommandType == "BMtranspar")
	{
		if (command->getParamsNum(ind) >= 5) //if number of params smaller than normal, then this is incorrect
			m_interface->updateElBMtranspar(command->getParam(ind, 2), command->getParam(ind, 3), stof(command->getParam(ind, 4))); // all params in order
	}
	else if (updCommandType == "BMselIntens")
	{
		if (command->getParamsNum(ind) >= 5) //if number of params smaller than normal, then this is incorrect
			m_interface->updateElBMselIntens(command->getParam(ind, 2), command->getParam(ind, 3), stof(command->getParam(ind, 4))); // all params in order
	}
	else if (updCommandType == "BMselCol")
	{
		if (command->getParamsNum(ind) >= 8) //if number of params smaller than normal, then this is incorrect
		{
			m_interface->updateElBMselCol(command->getParam(ind, 2), command->getParam(ind, 3), D3DXVECTOR4(stof(command->getParam(ind, 4)),
				stoi(command->getParam(ind, 5)), stof(command->getParam(ind, 6)), stof(command->getParam(ind, 7)))); // all params in order
		}
	}
	else if (updCommandType == "setBMvisible")
	{
		if (command->getParamsNum(ind) >= 5) //if number of params smaller than normal, then this is incorrect
			m_interface->setElBMvisible(command->getParam(ind, 2), command->getParam(ind, 3), stoi(command->getParam(ind, 4))); // all params in order
	}

	//Updating params of strings
	else if (updCommandType == "TSposX")
	{
		if (command->getParamsNum(ind) >= 6) //if number of params smaller than normal, then this is incorrect
		{
			return m_interface->updateElTSposX(m_D3D->GetDeviceContext(), command->getParam(ind, 2), command->getParam(ind, 3), stoi(command->getParam(ind, 4)),
				stoi(command->getParam(ind, 5))); // all params in order
		}
	}
	else if (updCommandType == "TSposY")
	{
		if (command->getParamsNum(ind) >= 6) //if number of params smaller than normal, then this is incorrect
		{
			return m_interface->updateElTSposY(m_D3D->GetDeviceContext(), command->getParam(ind, 2), command->getParam(ind, 3), stoi(command->getParam(ind, 4)),
				stoi(command->getParam(ind, 5))); // all params in order
		}
	}
	else if (updCommandType == "TStext")
	{
		if (command->getParamsNum(ind) >= 6) //if number of params smaller than normal, then this is incorrect
		{
			return m_interface->updateElTStext(m_D3D->GetDeviceContext(), command->getParam(ind, 2), command->getParam(ind, 3), stoi(command->getParam(ind, 4)), command->getParam(ind, 5)); // all params in order
		}
	}
	else if (updCommandType == "TScolor")
	{
		if (command->getParamsNum(ind) >= 8) //if number of params smaller than normal, then this is incorrect
		{
			return m_interface->updateElTScolor(m_D3D->GetDeviceContext(), command->getParam(ind, 2), command->getParam(ind, 3), atoi(command->getParam(ind, 4).c_str()),
				D3DXVECTOR4(stof(command->getParam(ind, 5)), stof(command->getParam(ind, 6)), stof(command->getParam(ind, 7)), stof(command->getParam(ind, 8))));
		}
	}
	else if (updCommandType == "TSadding")
	{
		if (command->getParamsNum(ind) >= 6) //if number of params smaller than normal, then this is incorrect
		{
			return  m_interface->updateElTSadding(m_D3D->GetDeviceContext(), command->getParam(ind, 2), command->getParam(ind, 3), stoi(command->getParam(ind, 4)), command->getParam(ind, 5)); // all params in order
		}
	}
	else if (updCommandType == "setTvisible")
	{
		if (command->getParamsNum(ind) >= 5) //if number of params smaller than normal, then this is incorrect
		{
			m_interface->setElTvisible(command->getParam(ind, 2), command->getParam(ind, 3), stoi(command->getParam(ind, 4))); // all params in order
		}
	}

	//Special slider commands
	else if (updCommandType == "nextSector")
	{
		m_interface->nextSector(command->getParam(0, 2));
	}
	else if (updCommandType == "backSector")
	{
		m_interface->backSector(command->getParam(0, 2));
	}

	//Special list commands
	else if (updCommandType == "addElement")
	{
		return m_interface->addElement(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), m_hwnd, command->getParam(0, 2));
	}
	else if (updCommandType == "deleteElement")
	{
		m_interface->deleteElement(m_D3D->GetDeviceContext(), command->getParam(0, 2), stoi(command->getParam(0, 3)));
	}
	else if (updCommandType == "setLastElement")
	{
		m_interface->setLastElement(m_D3D->GetDeviceContext(), command->getParam(0, 2), stoi(command->getParam(0, 3)));
	}
	else if (updCommandType == "clearElement")
	{
		m_interface->clear(command->getParam(0, 2));
	}
	//another commands
	else if (updCommandType == "setBMCommand")
	{
		m_interface->setBMCommand(command->getParam(0, 2), command->getParam(0, 3), command->getParam(0, 4), command->getParam(0, 5));
	}
	else if (updCommandType == "getSettings")
	{
		/*string filename = command->getParam(0, 6);
		filename.replace(filename.find_first_of("XXX___"), 6, m_resources->getSettings()->getLang());
		return m_interface->getSettings(command->getParam(0, 2), command->getParam(0, 3), stoi(command->getParam(0, 4)), command->getParam(0, 5),
			filename, m_D3D->GetDevice(), m_hwnd);*/
	}
}

bool GraphicsClass::updateGraphics(CommandClass * command,int ind)
{
	if (command->getCommandsNum() < 1)
		return false;
	std::string updCommandType = command->getParam(ind, 1); // type of update is second param

	if (updCommandType == "cameraPosition")
	{
		if (command->getParamsNum(ind) < 5) //if number of params smaller than normal, then this is incorrect
			return false;
		m_camera->SetPosition(D3DXVECTOR3(stof(command->getParam(ind, 2)), stof(command->getParam(ind, 3)), stof(command->getParam(ind, 4))));
		m_camera->Render();
	}
	if (updCommandType == "cameraRotation")
	{
		if (command->getParamsNum(ind) < 5) //if number of params smaller than normal, then this is incorrect
			return false;
		m_camera->SetRotation(D3DXVECTOR3(stof(command->getParam(ind, 2)), stof(command->getParam(ind, 3)), stof(command->getParam(ind, 4))));
		m_camera->Render();
	}
	if (updCommandType == "pick")
	{
		pick(stoi(command->getParam(ind, 2)), stoi(command->getParam(ind, 3)));
	}
	if (updCommandType == "unPick")
	{
		unPick(stoi(command->getParam(ind, 2)), stoi(command->getParam(ind, 3)));
	}
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

	//render test terrain
	for (int i = 0;i < TEST_NUM;i++)
	{
		m_test[i]->Render(m_shaderManager->getTerrainShader(), m_D3D->GetDeviceContext(), worldMatrix,
			viewMatrix, projectionMatrix, D3DXVECTOR3(0.0f, -1.0f, 0.5f), D3DXVECTOR4(0.15f, 0.15f, 0.15f, 1.0f),
			D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.5f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), 400.0f,
			SCREEN_DEPTH, 0);
	}

	//render 2D
	m_D3D->TurnZBufferOff();
	
	m_interface->Render(m_shaderManager->getInterfaceShader(), m_shaderManager->getFontShader(), m_D3D->GetDeviceContext(),
		worldMatrix, m_baseViewMatrix, orthoMatrix);

	m_D3D->TurnZBufferOn();

	
	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

void GraphicsClass::pick(int mouseX, int mouseY)
{
	CommandClass* command;
	std::string interfaceElName;
	int interfaceElInd;
	float posX, posY, posZ;
	D3DXVECTOR3 color;

	// First check for interface pick
	if (interfacePick(mouseX, mouseY, interfaceElInd, interfaceElName))
	{
		m_interface->pick(interfaceElInd, interfaceElName);
	}
}

void GraphicsClass::unPick(int mouseX, int mouseY)
{
	CommandClass* command;
	std::string interfaceElName;
	int interfaceElInd;
	float posX, posY, posZ;
	D3DXVECTOR3 color;

	// First check for interface pick
	if (interfacePick(mouseX, mouseY, interfaceElInd, interfaceElName))
	{
		m_interface->unPick(interfaceElInd, interfaceElName);
	}
}

bool GraphicsClass::interfacePick(int mouseX, int mouseY, int & ind, std::string & name)
{
	return m_interface->getEl(mouseX, mouseY, ind, name);
}
