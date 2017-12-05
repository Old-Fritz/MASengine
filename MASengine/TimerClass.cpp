///////////////////////////////////////////////////////////////////////////////
// Filename: timerclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "timerclass.h"

TimerClass::TimerClass()
{
}


TimerClass::TimerClass(const TimerClass& other)
{
}


TimerClass::~TimerClass()
{
}

bool TimerClass::Initialize()
{
	static int freq;
	// Check to see if this system supports high performance timers.
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	m_frequency = freq;
	if (m_frequency == 0)
	{
		LogManagerClass::getI().addLog("Error 2-1");
		return false;
	}

	// Find out how many times the frequency counter ticks every millisecond.
	m_ticksPerMs = (float)(m_frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);

	return true;
}

void TimerClass::Frame()
{
	INT64 currentTime;
	float timeDifference;


	QueryPerformanceCounter((LARGE_INTEGER*)& currentTime);

	timeDifference = (float)(currentTime - m_startTime);

	m_frameTime = timeDifference / m_ticksPerMs;

	m_startTime = currentTime;

	// count all counters
	for (auto counter = m_counters.begin();counter != m_counters.end();counter++)
	{
		counter->second.currentPos += counter->second.step * m_frameTime;
		if (counter->second.currentPos > counter->second.maxValue)
			counter->second.currentPos = 0;
	}

	return;
}

float TimerClass::GetTime()
{
	return m_frameTime;
}

void TimerClass::addCounter(std::string name, float step, float maxValue, float currentPos)
{
	CounterType counter;
	counter.step = step;
	counter.maxValue = maxValue;
	counter.currentPos = currentPos;

	m_counters.emplace(std::pair<int, CounterType>(Utils::getHash(name), counter));
}

float TimerClass::getCounter(std::string name)
{
	auto counter = m_counters.find(Utils::getHash(name));
	if (counter != m_counters.end())
		return counter->second.currentPos;
	else
		return 0;
}

void TimerClass::deleteCounter(std::string name)
{
	auto counter = m_counters.find(Utils::getHash(name));
	if (counter != m_counters.end())
		m_counters.erase(counter);
}
