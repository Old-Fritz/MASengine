////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_


///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define DIRECTINPUT_VERSION 0x0800

/////////////
// LINKING //
/////////////
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


//////////////
// INCLUDES //
//////////////
#include <dinput.h>
#include "GlobalManagerClass.h"



////////////////////////////////////////////////////////////////////////////////
// Class name: InputClass
////////////////////////////////////////////////////////////////////////////////
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE hinstance, HWND hwnd);
	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	void GetMouseLocation(int& mouseX, int& mouseY);
	unsigned char* GetKeyboardState();

	//set new commands
	void setPickCommand(int butNum, const std::string& command);
	void setUnPickCommand(int butNum, const std::string& command);
	void setUpWheelCommand(const std::string& command);
	void setDownWheelCommand(const std::string& command);

	//check buttons
	bool IsLeftMouseButtonDown();
	bool IsMiddleMouseButtonDown();
private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	unsigned char m_lastKeyboardState[256];
	DIMOUSESTATE m_lastMouseState;
	DIMOUSESTATE m_mouseState;

	std::string m_pickCommandsNames[259]; // Names of commands for every button
	std::string m_unPickCommandsNames[259]; // Names of commands for every button
	std::string m_upWheelCommand;
	std::string m_downWheelCommand;
	std::string m_commandsFileName;   // File with actions

	int m_mouseX, m_mouseY;
};

#endif