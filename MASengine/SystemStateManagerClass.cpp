////////////////////////////////////////////////////////////////////////////////
// Filename: SystemStateManagerClass.cpp
////////////////////////////////////////////////////////////////////////////////

#include "SystemStateManagerClass.h"

SystemStateManagerClass* SystemStateManagerClass::m_instance = 0;

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
	m_cpu = new(1) CpuClass;
	if (!m_cpu)
		return false;

	m_cpu->Initialize();

	//init fps
	m_fps = new(1) FpsClass;
	if (!m_fps)
		return false;
	m_fps->Initialize();

	//init timer
	m_timer = new(1) TimerClass;
	if (!m_timer)
		return false;
	result = m_timer->Initialize();
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 2-2");
		return false;
	}

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
		::operator delete(m_timer, sizeof(*m_timer), 1);
		m_timer = 0;
	}
	if (m_fps)
	{
		::operator delete(m_fps, sizeof(*m_fps), 1);
		m_fps = 0;
	}
	if (m_cpu)
	{
		m_cpu->Shutdown();
		::operator delete(m_cpu, sizeof(*m_cpu), 1);

		m_cpu = 0;
	}
}

SystemStateManagerClass & SystemStateManagerClass::getI()
{
	if (!m_instance)
		m_instance = new(1) SystemStateManagerClass;
	return *m_instance;
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
