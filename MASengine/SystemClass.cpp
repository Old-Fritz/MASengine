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
	result = m_input->Initialize(m_hinstance,m_hwnd);
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


	return true;
}
void SystemClass::Shutdown()
{
	//save settings first
	SettingsClass::getI().save();

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
	m_position->Move();

	//process graphics
	result = m_graphics->Frame(m_position->GetPosition(), m_position->GetRotation(), mouseX, mouseY); 
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 8-2");
		return false;
	}

	doCommands();

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
			//else if (commandType == "updateSystem")
			//	updateSystem(command, i);
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
