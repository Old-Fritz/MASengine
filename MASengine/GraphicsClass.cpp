#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
{
	m_LBMDown = false;

	m_D3D = 0;
	m_shaderManager = 0;
	m_camera = 0;
	m_interface = 0;
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

	//Init Interface
	m_interface = new(1) InterfaceClass;
	if (!result)
		return false;
	result = m_interface->Initialize(m_D3D->GetDevice(),m_D3D->GetDeviceContext(), hwnd, SettingsClass::getI().getIntParameter("ScreenWidth"), SettingsClass::getI().getIntParameter("ScreenHeight"));
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize test", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	if (m_interface)
	{
		m_interface->Shutdown();
		::operator delete(m_interface, sizeof(InterfaceClass), 1);
		m_interface = 0;
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

bool GraphicsClass::Frame(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int mouseX, int mouseY, bool isLBMDown)
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

	//check actions with LBM
	if (isLBMDown)
	{
		if (!m_LBMDown)
		{
			pick(mouseX, mouseY);
			m_LBMDown = true;
		}
	}
	else if (m_LBMDown)
	{
		m_LBMDown = false;
		unPick(mouseX, mouseY);
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
	std::string updCommandType = command->getParam(0, 1); // type of update is second param

													 //Updating params of element
	if (updCommandType == "setElVisible")
	{
		return m_Interface->setElVisible(command->getParam(0, 2), atoi(command->getParam(0, 3).c_str())); // all params in order
	}
	if (updCommandType == "startDrag")
	{
		m_Interface->startDrag(command->getParam(0, 2)); // all params in order
	}
	if (updCommandType == "endDrag")
	{
		m_Interface->endDrag(command->getParam(0, 2)); // all params in order
	}
	if (updCommandType == "elPos")
	{
		m_Interface->setPos(command->getParam(0, 2), stoi(command->getParam(0, 3)), stoi(command->getParam(0, 4))); // all params in order
	}


	//Updating params of bitmaps
	else if (updCommandType == "BMposX")
	{
		if (command->getParamsNum(0) >= 5) //if number of params smaller than normal, then this is incorrect
			m_Interface->updateElBMposX(command->getParam(0, 2), command->getParam(0, 3), atoi(command->getParam(0, 4).c_str())); // all params in order
	}
	else if (updCommandType == "BMposY")
	{
		if (command->getParamsNum(0) >= 5) //if number of params smaller than normal, then this is incorrect
			m_Interface->updateElBMposY(command->getParam(0, 2), command->getParam(0, 3), atoi(command->getParam(0, 4).c_str())); // all params in order
	}
	else if (updCommandType == "BMvertPercent")
	{
		if (command->getParamsNum(0) >= 5) //if number of params smaller than normal, then this is incorrect
			m_Interface->updateElBMvertPercent(command->getParam(0, 2), command->getParam(0, 3), atof(command->getParam(0, 4).c_str())); // all params in order
	}
	else if (updCommandType == "BMhorPercent")
	{
		if (command->getParamsNum(0) >= 5) //if number of params smaller than normal, then this is incorrect
			m_Interface->updateElBMhorPercent(command->getParam(0, 2), command->getParam(0, 3), atof(command->getParam(0, 4).c_str())); // all params in order
	}
	else if (updCommandType == "BMtranspar")
	{
		if (command->getParamsNum(0) >= 5) //if number of params smaller than normal, then this is incorrect
			m_Interface->updateElBMtranspar(command->getParam(0, 2), command->getParam(0, 3), atof(command->getParam(0, 4).c_str())); // all params in order
	}
	else if (updCommandType == "BMselIntens")
	{
		if (command->getParamsNum(0) >= 5) //if number of params smaller than normal, then this is incorrect
			m_Interface->updateElBMselIntens(command->getParam(0, 2), command->getParam(0, 3), atof(command->getParam(0, 4).c_str())); // all params in order
	}
	else if (updCommandType == "BMselCol")
	{
		if (command->getParamsNum(0) >= 8) //if number of params smaller than normal, then this is incorrect
		{
			m_Interface->updateElBMselCol(command->getParam(0, 2), command->getParam(0, 3), D3DXVECTOR4(atof(command->getParam(0, 4).c_str()),
				atoi(command->getParam(0, 5).c_str()), atof(command->getParam(0, 6).c_str()), atof(command->getParam(0, 7).c_str()))); // all params in order
		}
	}
	else if (updCommandType == "setBMvisible")
	{
		if (command->getParamsNum(0) >= 5) //if number of params smaller than normal, then this is incorrect
			return m_Interface->setElBMvisible(command->getParam(0, 2), command->getParam(0, 3), atoi(command->getParam(0, 4).c_str())); // all params in order
	}

	//Updating params of strings
	else if (updCommandType == "TSposX")
	{
		if (command->getParamsNum(0) >= 6) //if number of params smaller than normal, then this is incorrect
		{
			m_Interface->updateElTSposX(command->getParam(0, 2), command->getParam(0, 3), atoi(command->getParam(0, 4).c_str()),
				atoi(command->getParam(0, 5).c_str())); // all params in order
		}
	}
	else if (updCommandType == "TSposY")
	{
		if (command->getParamsNum(0) >= 6) //if number of params smaller than normal, then this is incorrect
		{
			m_Interface->updateElTSposY(command->getParam(0, 2), command->getParam(0, 3), atoi(command->getParam(0, 4).c_str()),
				atoi(command->getParam(0, 5).c_str())); // all params in order
		}
	}
	else if (updCommandType == "TStext")
	{
		if (command->getParamsNum(0) >= 9) //if number of params smaller than normal, then this is incorrect
		{
			int last; // ind of last el
			if (command->getParam(0, 6) == "end")
				last = m_Interface->getElTSsize(command->getParam(0, 2), command->getParam(0, 3), atoi(command->getParam(0, 4).c_str())) - 1; // get size of text
			else
				last = atoi(command->getParam(0, 6).c_str()); //else do like normal

			string text; // param of text
			text = m_resources->getTexts()->getText(command->getParam(0, 7), command->getParam(0, 8)); // get text by IEC command
			m_Interface->updateElTStext(command->getParam(0, 2), command->getParam(0, 3), atoi(command->getParam(0, 4).c_str()),
				atoi(command->getParam(0, 5).c_str()), last, text); // all params in order
		}
	}
	else if (updCommandType == "TScolor")
	{
		if (command->getParamsNum(0) >= 8) //if number of params smaller than normal, then this is incorrect
		{
			m_Interface->updateElTScolor(command->getParam(0, 2), command->getParam(0, 3), atoi(command->getParam(0, 4).c_str()),
				D3DXVECTOR3(atof(command->getParam(0, 5).c_str()), atof(command->getParam(0, 6).c_str()), atof(command->getParam(0, 7).c_str())));
		}
	}
	else if (updCommandType == "TSlastword")
	{
		if (command->getParamsNum(0) >= 7) //if number of params smaller than normal, then this is incorrect
		{
			string text; // param of text
			text = m_resources->getTexts()->getText(command->getParam(0, 5), command->getParam(0, 6)); // get text by IEC command
			m_Interface->updateElTSlastword(command->getParam(0, 2), command->getParam(0, 3), atoi(command->getParam(0, 4).c_str()), text); // all params in order
		}
	}
	else if (updCommandType == "setTvisible")
	{
		if (command->getParamsNum(0) >= 5) //if number of params smaller than normal, then this is incorrect
		{
			m_Interface->setElTvisible(command->getParam(0, 2), command->getParam(0, 3), atoi(command->getParam(0, 4).c_str())); // all params in order
		}
	}

	//Special slider commands
	else if (updCommandType == "nextSector")
	{
		return m_Interface->nextSector(command->getParam(0, 2));
	}
	else if (updCommandType == "backSector")
	{
		return m_Interface->backSector(command->getParam(0, 2));
	}

	//Special list commands
	else if (updCommandType == "addElement")
	{
		return m_Interface->addElement(command->getParam(0, 2), m_D3D->GetDevice(), m_hwnd);
	}
	else if (updCommandType == "deleteElement")
	{
		m_Interface->deleteElement(command->getParam(0, 2), stoi(command->getParam(0, 3)));
	}
	else if (updCommandType == "setLastElement")
	{
		m_Interface->setLastElement(command->getParam(0, 2), stoi(command->getParam(0, 3)));
	}
	else if (updCommandType == "clearElement")
	{
		m_Interface->clear(command->getParam(0, 2));
	}
	//another commands
	else if (updCommandType == "setBMCommand")
	{
		m_Interface->setBMCommand(command->getParam(0, 2), command->getParam(0, 3), command->getParam(0, 4), command->getParam(0, 5));
	}
	else if (updCommandType == "getSettings")
	{
		string filename = command->getParam(0, 6);
		filename.replace(filename.find_first_of("XXX___"), 6, m_resources->getSettings()->getLang());
		return m_Interface->getSettings(command->getParam(0, 2), command->getParam(0, 3), stoi(command->getParam(0, 4)), command->getParam(0, 5),
			filename, m_D3D->GetDevice(), m_hwnd);
	}

	command->Shutdown();
	command->Initialize("Nothing;");
	return command;
}

bool GraphicsClass::updateGraphics(CommandClass * command,int ind)
{
	if (command->getCommandsNum() < 1)
		return false;
	string updCommandType = command->getParam(0, 1); // type of update is second param

	if (updCommandType == "cameraPosition")
	{
		if (command->getParamsNum(0) < 5) //if number of params smaller than normal, then this is incorrect
			return false;
		m_Camera->SetPosition(D3DXVECTOR3(atof(command->getParam(0, 2).c_str()), atof(command->getParam(0, 3).c_str()), atof(command->getParam(0, 4).c_str())));
		m_Camera->Render();
	}
	if (updCommandType == "cameraRotation")
	{
		if (command->getParamsNum(0) < 5) //if number of params smaller than normal, then this is incorrect
			return false;
		m_Camera->SetRotation(D3DXVECTOR3(atof(command->getParam(0, 2).c_str()), atof(command->getParam(0, 3).c_str()), atof(command->getParam(0, 4).c_str())));
		m_Camera->Render();
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

	//render 2D
	m_D3D->TurnZBufferOff();
	
	m_interface->Render(m_shaderManager->getInterfaceShader(), m_shaderManager->getFontShader(), m_D3D->GetDeviceContext(), worldMatrix,orthoMatrix, viewMatrix);

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
