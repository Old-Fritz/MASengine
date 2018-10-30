#include "SystemClass.h"

SystemClass::SystemClass()
{
	m_input = 0;
	m_position = 0;
	m_graphics = 0;
	m_gameMech = 0;
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
	result = GM::getI().Initialize("data/init/init.txt");
	if (!result)
		return false;
	GM::LM()->addLog("All global system Initialized");

	// Initialize the windows api.
	InitializeWindows(SettingsClass::getI().getIntParameter("ScreenWidth"), SettingsClass::getI().getIntParameter("ScreenHeight"));
	GM::LM()->addLog("Windows Initialization");


	//Initialize input
	m_input = new(1) InputClass;
	if (!m_input)
		return false;
	result = m_input->Initialize(m_hinstance, m_hwnd);
	if (!result)
	{
		GM::LM()->addLog("Error 7-12");
		return false;
	}
	GM::LM()->addLog("input Initialization");

	//Initialize position
	m_position = new(1) PositionClass;
	if (!m_position)
		return false;
	GM::LM()->addLog("Position Initialization");

	//Initialize graphics
	m_graphics = new(1) GraphicsClass;
	if (!m_graphics)
		return false;
	result = m_graphics->Initialize(m_hwnd);
	if (!result)
	{
		GM::LM()->addLog("Error 8-1");
		return false;
	}
	GM::LM()->addLog("Graphics Initialization");

	//Initialize game mechanics
	m_gameMech = new(1) GameMechanicClass;
	if (!m_gameMech)
		return false;
	result = m_gameMech->Initialize();
	if (!result)
	{
		GM::LM()->addLog("Error 16-1");
		return false;
	}
	GM::LM()->addLog("Game Mechanics Initialization");

	
	
	// initialize resources of graphics
	result = m_graphics->InitializeResources();
	if (!result)
	{
		GM::LM()->addLog("Error 16-1");
		return false;
	}
	GM::LM()->addLog("Graphics resources Initialization");
	
	//end load and hide load screen
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

	m_graphics->ShutdownResources();

	//Shutdown game mechanic
	if (m_gameMech)
	{
		m_gameMech->Shutdown();
		::operator delete(m_gameMech, sizeof(*m_gameMech), 1);
		m_gameMech = 0;
	}

	//Shutdown graphics
	if (m_graphics)
	{
		m_graphics->Shutdown();
		::operator delete(m_graphics, sizeof(*m_graphics), 1);
		m_graphics = 0;
	}
	//Shutdown position
	if (m_position)
	{
		::operator delete(m_position, sizeof(*m_position), 1);
		m_position = 0;
	}
	//Shutdown Input
	if (m_input)
	{
		m_input->Shutdown();
		::operator delete(m_input, sizeof(*m_input), 1);
		m_input = 0;
	}

	//Shutdown other
	ShutdownWindows();
	GM::getI().Shutdown();
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
		GM::LM()->addLog("Error 7-13");
		return false;
	}

	//get mouse state
	m_input->GetMouseLocation(mouseX, mouseY);

	//process position
	m_position->SetFrameTime(SystemStateManagerClass::getI().getTimer()->GetTime());
	m_position->Move();

	//process graphics
	result = m_graphics->Frame(m_position->GetPosition(), m_position->GetRotation(), mouseX, mouseY); 
	if (!result)
	{
		GM::LM()->addLog("Error 8-2");
		return false;
	}

	//process all commands in queue
	result = doCommands();
	if (!result)
		//return false to exit game
		return false;
	
	//free frame memory
	MemoryManager::getI()->cleanOneFrame();

	return true;
}

bool SystemClass::doCommands()
{
	bool result;

	//process all commands while they are in queue
	while (CommandManagerClass::getI().isFull())
	{
		result = doSingleCommand(CommandManagerClass::getI().nextCommand());
		if (!result)
			//return false to exit game
			return false;
	}

	return true;
}

bool SystemClass::doSingleCommand(CommandClass * command)
{
	for (int i = 0; i < command->getCommandsNum(); i++)
	{
		//choose type of command by first param (next command start from next param`s index)
		auto commandEnum = CommandManagerClass::getI().getCommandEnum(Utils::getHash(command->getParam(i, 0)));
		switch (commandEnum)
		{
		case CommandManagerClass::updateInterface:
			m_graphics->updateInterface(command, i);
			break;
		case CommandManagerClass::updateGraphics:
			m_graphics->updateGraphics(command, i);
			break;
		case CommandManagerClass::updateSystem:
			updateSystem(command, i,1);
			break;
		case CommandManagerClass::operators:
			procesOperators(command, i, 1);
			break;
		case CommandManagerClass::get:
			get(command, i, 1);
			break;
		case CommandManagerClass::set:
			set(command, i, 1);
			break;
		case CommandManagerClass::reboot:
			GM::LM()->addLog("REBOOT");
			Shutdown();
			Initialize();
			break;
		case CommandManagerClass::stop:
			GM::LM()->addLog("EXIT");
			return false;
			break;
		default:
			break;
		}
		//else if (commandType == "updateTime")
		//	updateTime(command, i);
		//else if (commandType == "setParam")
		//	command->add(setParam(command->getSingleCommand(i)));
		//else if (commandType == "playSound")
		//	m_resources->getSound()->playSound(command->getParam(i, 1));
	}

	return true;
}

bool SystemClass::updateSystem(CommandClass * command, int ind, int firstCommand)
{
	int mouseX, mouseY;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	m_input->GetMouseLocation(mouseX, mouseY);


	if (command->getCommandsNum() < 1)
		return false;

	//choose type of command by her first param (next command start from next param`s index)
	auto commandEnum = CommandManagerClass::getI().getCommandEnum(Utils::getHash(command->getParam(ind, firstCommand)));

	switch (commandEnum)
	{
	case CommandManagerClass::position:
		if (command->getParamsNum(ind) < 4 + firstCommand) //if number of params smaller than normal, then this is incorrect
			return false;
		//get current position
		pos = m_position->GetRealPosition();
		rot = m_position->GetRotation();
		//make all changes to command
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
		//set new position
		m_position->SetPosition(D3DXVECTOR3(stof(command->getParam(ind, firstCommand+1)), stof(command->getParam(ind, firstCommand+2)), stof(command->getParam(ind, firstCommand+3))));
		break;
	case CommandManagerClass::rotation:
		if (command->getParamsNum(ind) < 4 + firstCommand) //if number of params smaller than normal, then this is incorrect
			return false;
		//get current rotation
		pos = m_position->GetRealPosition();
		rot = m_position->GetRotation();
		//make all changes to command
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
		//set new rotation
		m_position->SetRotation(D3DXVECTOR3(stof(command->getParam(ind, firstCommand+1)), stof(command->getParam(ind, firstCommand+2)), stof(command->getParam(ind, firstCommand+3))));
		break;
	case CommandManagerClass::setButCommand:
		if (command->getParamsNum(ind) < firstCommand+4)  //if number of params smaller than normal, then this is incorrect
			return false;
		//set new command by her type
		if (command->getParam(ind, firstCommand+2) == "pickCommand")
			m_input->setPickCommand(stoi(command->getParam(ind, firstCommand+1)), command->getParam(ind, firstCommand+3));
		else
			m_input->setUnPickCommand(stoi(command->getParam(ind, firstCommand+1)), command->getParam(ind, firstCommand+3));
		break;
	case CommandManagerClass::setWheelCommand:
		if (command->getParamsNum(ind) < firstCommand+3)  //if number of params smaller than normal, then this is incorrect
			return false;
		//set new command by her type
		if (command->getParam(ind, firstCommand+1) == "up")
			m_input->setUpWheelCommand(command->getParam(ind, firstCommand+2));
		else
			m_input->setDownWheelCommand(command->getParam(ind, firstCommand+2));
		break;
	case CommandManagerClass::setMoving:
		if (command->getParamsNum(ind) < firstCommand+3)  //if number of params smaller than normal, then this is incorrect
			return false;
		//set new move`s side
		m_position->setMove(stoi(command->getParam(ind, firstCommand+1)), stoi(command->getParam(ind, firstCommand+2)));
		break;
	default:
		break;
	}

	return true;
}

bool SystemClass::procesOperators(CommandClass * command, int ind, int firstCommand)
{
	//choose type of command by her first param (next command start from next param`s index)
	auto commandEnum = CommandManagerClass::getI().getCommandEnum(Utils::getHash(command->getParam(ind, firstCommand)));

	CommandClass* newCommand;

	switch (commandEnum)
	{
	case CommandManagerClass::IF:
		if (ConditionChecker.checkCondition(command->getParam(ind, firstCommand + 1)))
		{
			//do first command if condition is true
			newCommand = CommandManagerClass::getI().makeSingleCommand(command->getParam(ind, firstCommand + 2),
				PathManagerClass::getI().makePath(command->getParam(ind, firstCommand + 3)));
		}
		else
		{
			//do second command if condition is false
			newCommand = CommandManagerClass::getI().makeSingleCommand(command->getParam(ind, firstCommand + 4),
				PathManagerClass::getI().makePath(command->getParam(ind, firstCommand + 5)));
		}
		//process command in condition
		command->shareChanges(newCommand);
		doSingleCommand(newCommand);
		newCommand->shareChanges(command);
		break;
	default:
		break;
	}

	return true;
}

bool SystemClass::get(CommandClass * command, int ind, int firstCommand)
{
	std::string getValue;

	//choose type of command by her first param (next command start from next param`s index)
	auto commandEnum = CommandManagerClass::getI().getCommandEnum(Utils::getHash(command->getParam(ind, firstCommand)));

	switch (commandEnum)
	{
	case CommandManagerClass::getForward:
		command->addChange(command->getInitParam(ind, firstCommand + 2), command->getParam(ind, firstCommand + 1));
		break;
	case CommandManagerClass::getProvRegionId:
		//get data from game mechanic
		getValue = std::to_string(m_gameMech->getProvRegionID(GM::getI().getRegionTypeEnum(command->getParam(ind, firstCommand + 1)),
			std::stoi(command->getParam(ind, firstCommand + 2))));
		command->addChange(command->getInitParam(ind, firstCommand + 3), getValue);
		break;
	default:
		break;
	}


	return true;
}

bool SystemClass::set(CommandClass * command, int ind, int firstCommand)
{
	//choose type of command by her first param (next command start from next param`s index)
	auto commandEnum = CommandManagerClass::getI().getCommandEnum(Utils::getHash(command->getParam(ind, firstCommand)));

	switch (commandEnum)
	{
	case CommandManagerClass::setProvRegion:
		//make changes in game mechanic
		m_gameMech->setProvRegion(GM::getI().getRegionTypeEnum(command->getParam(ind, firstCommand + 1)),
			std::stoi(command->getParam(ind, firstCommand + 2)),
			std::stoi(command->getParam(ind, firstCommand + 3)));
		break;
	default:
		break;
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
