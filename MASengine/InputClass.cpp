////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "inputclass.h"

InputClass::InputClass()
{
	m_directInput = 0;
	m_keyboard = 0;
	m_mouse = 0;
	m_commandsFileName = 0;
}

InputClass::InputClass(const InputClass& other)
{
}

InputClass::~InputClass()
{
}

bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd)
{
	HRESULT result;


	// Store file with commands for input
	m_commandsFileName = SettingsClass::getI().getPathParameter("ButCommandFilename");

	// Initialize the location of the mouse on the screen.
	m_mouseX = 0;
	m_mouseY = 0;

	// Initialize the main direct input interface.
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
	{
		GM::LM()->addLog("Error 7-1");
		return false;
	}

	// Initialize the direct input interface for the keyboard.
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		GM::LM()->addLog("Error 7-2");
		return false;
	}

	// Set the data format.  In this case since it is a keyboard we can use the predefined data format.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		GM::LM()->addLog("Error 7-3");
		return false;
	}

	// Set the cooperative level of the keyboard to not share with other programs.
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		GM::LM()->addLog("Error 7-4");
		return false;
	}

	// Now acquire the keyboard.
	result = m_keyboard->Acquire();
	if (FAILED(result))
	{
		GM::LM()->addLog("Error 7-5");
		return false;
	}

	// Initialize the direct input interface for the mouse.
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
	{
		GM::LM()->addLog("Error 7-6");
		return false;
	}

	// Set the data format for the mouse using the pre-defined mouse data format.
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		GM::LM()->addLog("Error 7-7");
		return false;
	}

	// Set the cooperative level of the mouse to share with other programs.
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		GM::LM()->addLog("Error 7-8");
		return false;
	}

	// Acquire the mouse.
	result = m_mouse->Acquire();
	if (FAILED(result))
	{
		GM::LM()->addLog("Error 7-9");
		return false;
	}

	//set base commands for all buttons
	for (int i = 0; i < 259; i++)
	{
		m_pickCommandsNames[i] = std::to_string(i) + "pickCommand";
		m_unPickCommandsNames[i] = std::to_string(i) + "unPickCommand";
		m_keyboardState[i] = 0;
	}
	m_upWheelCommand = "upWheelCommand";
	m_downWheelCommand = "downWheelCommand";


	return true;
}

void InputClass::Shutdown()
{
	// Release the mouse.
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	// Release the keyboard.
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	// Release the main interface to direct input.
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}

	return;
}

bool InputClass::Frame()
{
	bool result;

	//save last state
	for (int i = 0; i < 256; i++)
	{
		m_lastKeyboardState[i] = m_keyboardState[i];
	}

	m_lastMouseState = m_mouseState;

	// Read the current state of the keyboard.
	result = ReadKeyboard();
	if (!result)
	{
		GM::LM()->addLog("Error 7-10");
		return false;
	}

	// Read the current state of the mouse.
	result = ReadMouse();
	if (!result)
	{
		GM::LM()->addLog("Error 7-11");
		return false;
	}

	// Process the changes in the mouse and keyboard.
	ProcessInput();

	//check for press and add commands
	//check keyboard
	for (int i = 0; i < 256; i++)
	{
		if ((bool)m_keyboardState[i] != (bool)m_lastKeyboardState[i]) //check for changing
		{

			if (m_keyboardState[i]) // was pick
			{
				CommandManagerClass::getI().addCommand(m_pickCommandsNames[i], m_commandsFileName);
			}
			else //was unPick
			{
				CommandManagerClass::getI().addCommand(m_unPickCommandsNames[i], m_commandsFileName);
			}
		}
	}
	//check mouse buttons
	for (int i = 0; i < 3; i++)
	{
		if ((bool)m_mouseState.rgbButtons[i] != (bool)m_lastMouseState.rgbButtons[i]) //check for changing
		{

			if (m_mouseState.rgbButtons[i]) // was pick
			{
				CommandManagerClass::getI().addCommand(m_pickCommandsNames[i+256], m_commandsFileName);	
			}
			else //was unPick
			{
				CommandManagerClass::getI().addCommand(m_unPickCommandsNames[i+256], m_commandsFileName);
			}
			CommandManagerClass::getI().addChange("mouseX", m_mouseX);
			CommandManagerClass::getI().addChange("mouseY", m_mouseY);

		}
	}
	if (m_mouseState.lZ >= 0)
	{
		for (int i = 0; i < m_mouseState.lZ / 120; i++)
		{
			CommandManagerClass::getI().addCommand(m_upWheelCommand, m_commandsFileName);
		}
	}
	else
	{
		for (int i = 0; i < abs(m_mouseState.lZ) / 120; i++)
		{
			CommandManagerClass::getI().addCommand(m_downWheelCommand, m_commandsFileName);
		}
	}

	return true;
}

bool InputClass::ReadKeyboard()
{
	HRESULT result;


	// Read the keyboard device.
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool InputClass::ReadMouse()
{
	HRESULT result;


	// Read the mouse device.
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void InputClass::ProcessInput()
{
	// Update the location of the mouse cursor based on the change of the mouse location during the frame.
	m_mouseX += m_mouseState.lX*(SettingsClass::getI().getIntParameter("MouseSensitivity") / 50.0f + 0.2f);
	m_mouseY += m_mouseState.lY*(SettingsClass::getI().getIntParameter("MouseSensitivity") / 50.0f + 0.2f);

	// Ensure the mouse location doesn't exceed the screen width or height.
	if (m_mouseX < 0)
		m_mouseX = 0; 
	if (m_mouseY < 0) 
		m_mouseY = 0;

	if (m_mouseX >SettingsClass::getI().getIntParameter("ScreenWidth"))
		m_mouseX = SettingsClass::getI().getIntParameter("ScreenWidth");
	if (m_mouseY > SettingsClass::getI().getIntParameter("ScreenHeight"))
		m_mouseY = SettingsClass::getI().getIntParameter("ScreenHeight");

	return;
}

bool InputClass::IsEscapePressed()
{
	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if (m_keyboardState[DIK_ESCAPE] & 0x80)
	{
		return true;
	}

	return false;
}

void InputClass::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;
	return;
}

unsigned char* InputClass::GetKeyboardState()
{
	return m_keyboardState;
}

bool InputClass::IsLeftMouseButtonDown()
{
	// Check if the left mouse button is currently pressed.
	if (m_mouseState.rgbButtons[0] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputClass::IsMiddleMouseButtonDown()
{
	// Check if the left mouse button is currently pressed.
	if (m_mouseState.rgbButtons[2] & 0x80)
	{
		return true;
	}

	return false;
}

void InputClass::setPickCommand(int butNum, const std::string& command)
{
	if (butNum < 259)
		m_pickCommandsNames[butNum] = command;
		
}

void InputClass::setUnPickCommand(int butNum, const std::string& command)
{
	if (butNum < 259)
		m_unPickCommandsNames[butNum] = command;
}

void InputClass::setUpWheelCommand(const std::string& command)
{
	m_upWheelCommand = command;
}

void InputClass::setDownWheelCommand(const std::string& command)
{
	m_downWheelCommand = command;
}
