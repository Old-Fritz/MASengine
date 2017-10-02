#include "SystemClass.h"

SystemClass::SystemClass()
{
	m_input = 0;
	m_position = 0;
	m_graphics = 0;
}
SystemClass::SystemClass(const SystemClass &)
{
}
SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
	bool result;

	srand(time(0));

	// Initialize GlobalManager
	result = GlobalManagerClass::getI().Initialize("data/init/init.txt");
	if (!result)
		return false;
	LogManagerClass::getI().addLog("All global system Initialized");

	// Initialize the windows api.
	InitializeWindows(SettingsClass::getI().getIntParameter("ScreenWidth"), SettingsClass::getI().getIntParameter("ScreenHeight"));
	LogManagerClass::getI().addLog("Windows Initialization");


	//Initialize input
	m_input = new(1) InputClass;
	if (!m_input)
		return false;
	result = m_input->Initialize(m_hinstance, m_hwnd);
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 7-12");
		return false;
	}
	LogManagerClass::getI().addLog("input Initialization");

	//Initialize position
	m_position = new(1) PositionClass;
	if (!m_position)
		return false;
	LogManagerClass::getI().addLog("Position Initialization");

	//Initialize graphics
	m_graphics = new(1) GraphicsClass;
	if (!m_graphics)
		return false;
	result = m_graphics->Initialize(m_hwnd);
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 8-1");
		return false;
	}
	LogManagerClass::getI().addLog("Graphics Initialization");

	//Initialize prov manager
	if (&(ProvManagerClass::getI()))
	{
		result = ProvManagerClass::getI().Initialize(SettingsClass::getI().getPathParameter("ProvFilename"));
		if (!result)
		{
			return false;
		}
	}
	LogManagerClass::getI().addLog("Provs Initialization");


	
	LoadScreenManagerClass::getI().hideElements();

	//make start command
	CommandManagerClass::getI().addCommand("startAction", SettingsClass::getI().getPathParameter("GeneralActionsFilename"));
	doCommands();

	return true;
}
void SystemClass::Shutdown()
{
	//save settings first
	SettingsClass::getI().save();

	if (&(ProvManagerClass::getI()))
	{
		ProvManagerClass::getI().Shutdown();
	}

	//Shutdown graphics
	if (m_graphics)
	{
		m_graphics->Shutdown();
		::operator delete(m_graphics, sizeof(GraphicsClass), 1);
		m_graphics = 0;
	}
	//Shutdown position
	if (m_position)
	{
		::operator delete(m_position, sizeof(PositionClass), 1);
		m_position = 0;
	}
	//Shutdown Input
	if (m_input)
	{
		m_input->Shutdown();
		::operator delete(m_input, sizeof(InputClass), 1);
		m_input = 0;
	}

	//Shutdown other
	ShutdownWindows();
	GlobalManagerClass::getI().Shutdown();
}
void SystemClass::Run()
{
	MSG msg;
	bool done, result;


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
			done = true;
		else
		{
			// Otherwise do the frame processing.
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}

		// Check if the user pressed escape and wants to quit.
		//if (m_Input->IsEscapePressed() == true)
		//{
		//	done = true;
		//}

	}

	return;
}

bool SystemClass::Frame()
{
	bool result;
	int mouseX, mouseY;

	SystemStateManagerClass::getI().Frame();

	//process input
	result = m_input->Frame();
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 7-13");
		return false;
	}

	m_input->GetMouseLocation(mouseX, mouseY);

	//process position
	m_position->SetFrameTime(SystemStateManagerClass::getI().GetTime());
	m_position->Move();

	//process graphics
	result = m_graphics->Frame(m_position->GetPosition(), m_position->GetRotation(), mouseX, mouseY); 
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 8-2");
		return false;
	}

	result = doCommands();
	if (!result)
		return false;

	return true;
}

bool SystemClass::doCommands()
{
	CommandClass* command;

	while (CommandManagerClass::getI().isFull())
	{
		command = CommandManagerClass::getI().nextCommand();

		for (int i = 0; i < command->getCommandsNum(); i++)
		{
			std::string commandType = command->getParam(i, 0);
			if (commandType == "updateInterface")
				m_graphics->updateInterface(command,i);
			else if (commandType == "updateGraphics")
				m_graphics->updateGraphics(command, i);
			else if (commandType == "updateSystem")
				updateSystem(command, i);
			//else if (commandType == "updateTime")
			//	updateTime(command, i);
			//else if (commandType == "setParam")
			//	command->add(setParam(command->getSingleCommand(i)));
			//else if (commandType == "playSound")
			//	m_resources->getSound()->playSound(command->getParam(i, 1));
			else if (commandType == "reboot")
			{
				Shutdown();
				Initialize();
			}
			else if (commandType == "stop")
				return false;
		}
	}

	return true;
}

bool SystemClass::updateSystem(CommandClass * command, int ind)
{
	int mouseX, mouseY;
	m_input->GetMouseLocation(mouseX, mouseY);


	if (command->getCommandsNum() < 1)
		return false;
	std::string updCommandType = command->getParam(0, 1); // type of update is second param

	if (updCommandType == "cameraPosition")
	{
		if (command->getParamsNum(0) < 5) //if number of params smaller than normal, then this is incorrect
			return false;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		pos = m_position->GetRealPosition();
		rot = m_position->GetRotation();
		command->addChange("posX", pos.x);
		command->addChange("posY", pos.y);
		command->addChange("posZ", pos.z);
		command->addChange("rotX", rot.x);
		command->addChange("rotY", rot.y);
		command->addChange("rotZ", rot.z);
		command->addChange("screenWidth", SettingsClass::getI().getIntParameter("ScreenWidth"));
		command->addChange("screenHeight", SettingsClass::getI().getIntParameter("ScreenHeight"));
		command->addChange("mouseX", mouseX);
		command->addChange("mouseY", mouseY);
		m_position->SetPosition(D3DXVECTOR3(stof(command->getParam(0, 2)), stof(command->getParam(0, 3)), stof(command->getParam(0, 4))));
	}
	else if (updCommandType == "cameraRotation")
	{
		if (command->getParamsNum(0) < 5) //if number of params smaller than normal, then this is incorrect
			return false;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		pos = m_position->GetRealPosition();
		rot = m_position->GetRotation();
		command->addChange("posX", pos.x);
		command->addChange("posY", pos.y);
		command->addChange("posZ", pos.z);
		command->addChange("rotX", rot.x);
		command->addChange("rotY", rot.y);
		command->addChange("rotZ", rot.z);
		command->addChange("screenWidth", SettingsClass::getI().getIntParameter("ScreenWidth"));
		command->addChange("screenHeight", SettingsClass::getI().getIntParameter("ScreenHeight"));
		command->addChange("mouseX", mouseX);
		command->addChange("mouseY", mouseY);
		m_position->SetRotation(D3DXVECTOR3(stof(command->getParam(0, 2)), stof(command->getParam(0, 3)), stof(command->getParam(0, 4))));

	}
	else if (updCommandType == "setButCommand")
	{
		if (command->getParamsNum(0) < 5)
			return false;
		if (command->getParam(0, 3) == "pickCommand")
			m_input->setPickCommand(stoi(command->getParam(0, 2)), command->getParam(0, 4));
		else
			m_input->setUnPickCommand(stoi(command->getParam(0, 2)), command->getParam(0, 4));
	}
	else if (updCommandType == "setWheelCommand")
	{
		if (command->getParamsNum(0) < 4)
			return false;
		if (command->getParam(0, 2) == "up")
			m_input->setUpWheelCommand(command->getParam(0, 3));
		else
			m_input->setDownWheelCommand(command->getParam(0, 3));
	}
	else if (updCommandType == "setMoving")
	{
		if (command->getParamsNum(0) < 4)
			return false;
		m_position->setMove(stoi(command->getParam(0, 2)), stoi(command->getParam(0, 3)));
	}
	
	return true;
}

void SystemClass::InitializeWindows(int screenWidth, int screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;
	//DEVMODEW dmScreenSettings;

	// Get an external pointer to this object.	
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"Demo";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Set the position of the window to the top left corner.
	posX = posY = 0;

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (SettingsClass::getI().getIntParameter("Fullscreen"))
	{
		// Determine the resolution of the clients desktop screen.
		//screenWidth = GetSystemMetrics(SM_CXSCREEN);
		//screenHeight = GetSystemMetrics(SM_CYSCREEN);

		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);


	}

	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);



	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}

void SystemClass::ShutdownWindows()
{

	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (SettingsClass::getI().getIntParameter("Fullscreen"))
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}

LRESULT SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}
