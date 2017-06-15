////////////////////////////////////////////////////////////////////////////////
// Filename: SystemStateManagerClass.cpp
////////////////////////////////////////////////////////////////////////////////

#include "SystemStateManagerClass.h"

SystemStateManagerClass::SystemStateManagerClass()
{
	m_cpu = 0;
	m_fps = 0;
	m_timer = 0;
}
SystemStateManagerClass::SystemStateManagerClass(const SystemStateManagerClass &)
{
}
SystemStateManagerClass::~SystemStateManagerClass()
{
}

bool SystemStateManagerClass::Initialize()
{
	bool result;

	//init cpu
	m_cpu = (CpuClass*)MemoryManager.getStackMemory(sizeof(CpuClass));
	if (!m_cpu)
		return false;

	m_cpu->Initialize();

	//init fps
	m_fps = (FpsClass*)MemoryManager.getStackMemory(sizeof(FpsClass));
	if (!m_fps)
		return false;
	m_fps->Initialize();

	//init timer
	m_timer = (TimerClass*)MemoryManager.getStackMemory(sizeof(TimerClass));
	if (!m_timer)
		return false;
	result = m_timer->Initialize();
	if (!result)
	{
		LogManager.addLog("Error 2-2");
	}
		return false;

	return true;
}
void SystemStateManagerClass::Frame()
{
	m_cpu->Frame();
	m_fps->Frame();
	m_timer->Frame();
}
void SystemStateManagerClass::Shutdown()
{
	if (m_timer)
	{
		MemoryManager.deleteStack(m_timer, sizeof(*m_timer));
		m_timer = 0;
	}
	if (m_fps)
	{
		MemoryManager.deleteStack(m_fps, sizeof(*m_fps));
		m_fps = 0;
	}
	if (m_cpu)
	{
		m_cpu->Shutdown();
		MemoryManager.deleteStack(m_cpu, sizeof(*m_cpu));
		m_cpu = 0;
	}
}

float SystemStateManagerClass::GetTime()
{
	return m_timer->GetTime();
}
int SystemStateManagerClass::GetCpuPercentage()
{
	return m_cpu->GetCpuPercentage();
}
int SystemStateManagerClass::GetFps()
{
	return m_fps->GetFps();
}
