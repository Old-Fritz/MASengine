#pragma once
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

#include "MemoryManagerClass.h"


#include "graphicsclass.h"
#include "inputclass.h"
#include "fpsclass.h"
#include "cpuclass.h"
#include "timerclass.h"
#include "soundclass.h"
#include "positionclass.h"
#include "WeatherClass.h"


/////////////
// GLOBALS //
/////////////

const int MAX_PROVS = 39000;

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

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool gameFrame();
	bool Frame();
	void InitializeWindows(int screenWidth, int screenHeight);
	void ShutdownWindows();
	bool updateSystem(CommandClass* command);
	bool updateTime(CommandClass* command);
	bool monthTask();
	CommandClass* setParam(CommandClass* command);
	void HandleMouseMove(float, int, int);
	bool doCommand(CommandClass* command);

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	ResourceManagerClass* m_resources;
	InputClass* m_Input;
	GraphicsClass* m_Graphics;
	FpsClass* m_Fps;
	CpuClass* m_Cpu;
	TimerClass* m_Timer;
	PositionClass* m_Position;

	ProvManagerClass* m_provManager;
	FractionManagerClass* m_fractionManager;
	WeatherClass* m_weather;

	D3DXVECTOR4 m_weatherColors[4000];

	int m_lastx;
	int m_lasty;
	float m_Xcamspeed, m_Ycamspeed;
};


/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


/////////////
// GLOBALS //
/////////////
static SystemClass* ApplicationHandle = 0;


#endif