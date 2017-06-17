////////////////////////////////////////////////////////////////////////////////
// Filename: SystemStateManagerClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SYSTEMSTATEMANAGERCLASS_H_
#define _SYSTEMSTATEMANAGERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include "CpuClass.h"
#include "FpsClass.h"
#include "TimerClass.h"
#include "MemoryManagerClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: SystemStateManagerClass
////////////////////////////////////////////////////////////////////////////////
class SystemStateManagerClass
{
public:
	SystemStateManagerClass();
	SystemStateManagerClass(const SystemStateManagerClass&);
	~SystemStateManagerClass();

	bool Initialize();
	void Frame();
	void Shutdown();

	float GetTime();
	int GetCpuPercentage();
	int GetFps();
private:
	CpuClass* m_cpu;
	FpsClass* m_fps;
	TimerClass* m_timer;
};

/////////////
// GLOBALS //
/////////////
static SystemStateManagerClass SystemStateManager;

#endif