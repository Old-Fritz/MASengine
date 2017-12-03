////////////////////////////////////////////////////////////////////////////////
// Filename: timerclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TIMERCLASS_H_
#define _TIMERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <windows.h>
#include "LogManagerClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: TimerClass
////////////////////////////////////////////////////////////////////////////////
/*!
\addtogroup SystemStateManagment
@{
*/
/*!
Класс для получения сведений о точном межкадровом времени
*/
class TimerClass
{
public:
	TimerClass();
	TimerClass(const TimerClass&);
	~TimerClass();

	//! Инициализировать класс \return false, если были ошибки
	bool Initialize();
	//! Обновить данные, выполняется на каждом кадре
	void Frame();

	//! Получить точное время между кадрами \return время между кадрами в мс
	float GetTime();
private:
	INT64 m_frequency;
	float m_ticksPerMs;
	INT64 m_startTime;
	float m_frameTime;
};
/*! @} */
#endif