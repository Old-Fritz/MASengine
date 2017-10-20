#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
{

	m_D3D = 0;
	m_shaderManager = 0;
	m_camera = 0;
	m_interface = 0;
	m_light = 0;
	m_terrain = 0;


	m_lightPos = 0;
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
	m_screenHeight = SettingsClass::getI().getIntParameter("ScreenHeight");
	m_screenWidth = SettingsClass::getI().getIntParameter("ScreenWidth");

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

	//Initialize light
	m_light = new(1) LightClass;
	if (!m_light)
		return false;

	m_light->SetAmbientColor(D3DXVECTOR4(0.05f, 0.05f, 0.05f, 1.0f));
	m_light->SetDiffuseColor(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	m_light->SetDirection(D3DXVECTOR3(0.9f, 0.0f, 0.0f));
	m_light->SetSpecularColor(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	m_light->SetSpecularPower(400.0f);

	//Init loadscreen manager
	result = LoadScreenManagerClass::getI().Initialize(m_D3D,m_shaderManager,m_baseViewMatrix,m_hwnd,SettingsClass::getI().getPathParameter("loadScreenManagerFilename"));
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 9-20");
		return false;
	}
	LoadScreenManagerClass::getI().showElements();

	

	

	return true;
}

bool GraphicsClass::InitializeResources()
{
	bool result;

	//Init Interface
	m_interface = new(1) InterfaceClass;
	if (!m_interface)
		return false;
	result = m_interface->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), m_hwnd, m_screenWidth, m_screenHeight);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize interface", L"Error", MB_OK);
		return false;
	}


	m_terrain = new(1) TerrainManagerClass;
	if (!m_terrain)
		return false;

	result = m_terrain->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(),
		SettingsClass::getI().getStrParameter("TerrainFilenameBase"), SettingsClass::getI().getIntParameter("NumOFBlocks"));
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize terrain", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	

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

void GraphicsClass::ShutdownResources()
{

	if (m_terrain)
	{
		m_terrain->Shutdown();
		::operator delete(m_terrain, sizeof(*m_terrain), 1);
		m_terrain = 0;
	}

	if (m_interface)
	{
		m_interface->Shutdown();
		::operator delete(m_interface, sizeof(InterfaceClass), 1);
		m_interface = 0;
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

	m_lightPos += 0.0001f * SystemStateManagerClass::getI().GetTime();
	if (m_lightPos > 1.0f)
		m_lightPos = -1.0f;

	m_light->SetDirection(D3DXVECTOR3(m_lightPos,-0.5f,0));

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
	FrustumClass* frustum = new(2) FrustumClass;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));

	// Generate the view matrix based on the camera's position.
	m_camera->Render();

	// Get the world, view, projection, and ortho matrices from the camera and d3d objects.
	m_camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	//construct frustum
	frustum->ConstructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);

	//render test terrain

	m_terrain->Render(m_shaderManager->getTerrainShader(), m_D3D->GetDeviceContext(), worldMatrix,
			viewMatrix, projectionMatrix, m_light->GetDirection(), m_light->GetAmbientColor(),
			m_light->GetDiffuseColor(), m_camera->GetPosition(), m_light->GetSpecularColor(),
			m_light->GetSpecularPower(), SCREEN_DEPTH, frustum);
	

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

	D3DXVECTOR3 point;
	int provNum;

	// First check for interface pick
	if (interfacePick(mouseX, mouseY, interfaceElInd, interfaceElName))
	{
		m_interface->pick(interfaceElInd, interfaceElName);
	}
	else if (terrainPick(mouseX, mouseY, provNum, point))
	{
		
		return;
	}
}

void GraphicsClass::unPick(int mouseX, int mouseY)
{
	CommandClass* command;
	std::string interfaceElName;
	int interfaceElInd;
	float posX, posY, posZ;

	D3DXVECTOR3 point;
	int provNum;

	// First check for interface pick
	if (interfacePick(mouseX, mouseY, interfaceElInd, interfaceElName))
	{
		m_interface->unPick(interfaceElInd, interfaceElName);
	}
	else if (terrainPick(mouseX, mouseY, provNum, point))
	{
		if (!ProvManagerClass::getI().getProv(provNum)->getRegions(GlobalManagerClass::NATION).size()
	|| ProvManagerClass::getI().getProv(provNum)->getRegions(GlobalManagerClass::NATION).back())
			ProvRegionManagerClass::getI().getProvRegion(GlobalManagerClass::NATION,0)->add(provNum);
		else
			ProvRegionManagerClass::getI().getProvRegion(GlobalManagerClass::NATION, 1)->add(provNum);

		return;
	}
}

bool GraphicsClass::interfacePick(int mouseX, int mouseY, int & ind, std::string & name)
{
	return m_interface->getEl(mouseX, mouseY, ind, name);
}

bool GraphicsClass::terrainPick(int mouseX, int mouseY, int& provNum, D3DXVECTOR3& point)
{
	D3DXVECTOR3 rayOrigin, rayDirection;
	bool result;

	//create intersection ray
	createRay(mouseX, mouseY, rayOrigin, rayDirection);

	for (int i = 0;i < TEST_NUM;i++)
	{
		result = m_terrain->pick(m_D3D->GetDeviceContext(), rayOrigin, rayDirection, provNum, point);
		if (result)
			return true;
	}

	return false;
}

void GraphicsClass::createRay(int mouseX, int mouseY, D3DXVECTOR3 & rayOrigin, D3DXVECTOR3 & rayDirection)
{
	D3DXMATRIX projectionMatrix, viewMatrix, inverseViewMatrix;
	D3DXVECTOR3 vec;
	bool intersect, result;
	UINT hitcount, faceIndex;
	float u, v, dist;
	ID3D10Blob* allHits;

	m_D3D->GetProjectionMatrix(projectionMatrix);

	//create unusial vector from old algoritm without comments (((( (it has worked)
	vec.x = (((2.0f * (float)mouseX) / (float)m_screenWidth - 1) / projectionMatrix._11);
	vec.y = -(((2.0f * (float)mouseY) / (float)m_screenHeight - 1) / projectionMatrix._22);
	vec.z = 1.0f;

	// Get the inverse of the view matrix.
	m_camera->GetViewMatrix(viewMatrix);
	D3DXMatrixInverse(&inverseViewMatrix, NULL, &viewMatrix);

	// Calculate the direction of the picking ray in view space.
	rayDirection.x = vec.x*inverseViewMatrix._11 + vec.y*inverseViewMatrix._21 + vec.z*inverseViewMatrix._31;
	rayDirection.y = vec.x*inverseViewMatrix._12 + vec.y*inverseViewMatrix._22 + vec.z*inverseViewMatrix._32;
	rayDirection.z = vec.x*inverseViewMatrix._13 + vec.y*inverseViewMatrix._23 + vec.z*inverseViewMatrix._33;

	// Normalize the ray direction.
	D3DXVec3Normalize(&rayDirection, &rayDirection);

	// Get the origin of the picking ray which is the position of the camera.
	rayOrigin = m_camera->GetPosition();

	rayOrigin.x = inverseViewMatrix._41;
	rayOrigin.y = inverseViewMatrix._42;
	rayOrigin.z = inverseViewMatrix._43;

	return;
}

