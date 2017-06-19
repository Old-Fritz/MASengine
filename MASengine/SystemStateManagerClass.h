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
	

	bool Initialize();
	void Frame();
	void Shutdown();

	static SystemStateManagerClass& getI();

	float GetTime();
	int GetCpuPercentage();
	int GetFps();
private:
	SystemStateManagerClass();
	SystemStateManagerClass(const SystemStateManagerClass&);
	~SystemStateManagerClass();
private:
	CpuClass* m_cpu;
	FpsClass* m_fps;
	TimerClass* m_timer;

	static SystemStateManagerClass* m_instance;
};

#endif