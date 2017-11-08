////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_


///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define WIN32_LEAN_AND_MEAN


//////////////
// INCLUDES //
//////////////
#include <windows.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////

#include "GlobalManagerClass.h"
#include "InputClass.h"
#include "PositionClass.h"
#include "GraphicsClass.h"
#include "GameMechanicClass.h"
#include "ProvRegionManagerClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: SystemClass
////////////////////////////////////////////////////////////////////////////////
class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

private:
	bool Frame();
	void InitializeWindows(int screenWidth, int screenHeight);
	void ShutdownWindows();

	//commands
	bool doCommands();
	bool doSingleCommand(CommandClass* command);
	bool updateSystem(CommandClass* command, int ind,int firstCommand);
	bool procesOperators(CommandClass* command, int ind, int firstCommand);
	bool get(CommandClass* command, int ind, int firstCommand);
	bool set(CommandClass* command, int ind, int firstCommand);
private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	//blocks
	InputClass* m_input;
	PositionClass* m_position;
	GraphicsClass* m_graphics;
	GameMechanicClass* m_gameMech;

};


/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);


/////////////
// GLOBALS //
/////////////
static SystemClass* ApplicationHandle = 0;


#endif