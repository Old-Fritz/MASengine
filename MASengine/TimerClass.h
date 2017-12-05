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
#include<map>
#include "Utils.h"

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
private:
	//! Тип счетчика
	struct CounterType
	{
		float step; //!< Шаг, с которым смещается уровень счетчика за 1мс
		float maxValue; //!< Максимальное значение счетчика
		float currentPos; //!< Уровень счетчика
	};
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

	//! Добавить счетчик \param[in] name - имя счетчика  \param[in] step - Шаг, с которым смещается уровень счетчика за 1мс 
	//! \param[in] maxValue - Максимальное значение счетчика \param[in] currentPos - Уровень счетчика
	void addCounter(std::string name, float step, float maxValue, float currentPos);

	//! Получить уровень счетчика \param[in] name - имя счетчика
	float getCounter(std::string name);

	//! Удалить счетчик
	void deleteCounter(std::string name);

private:
	INT64 m_frequency;
	float m_ticksPerMs;
	INT64 m_startTime;
	float m_frameTime;

	std::map<int, CounterType> m_counters;  //!< Глобальные счетчики
};
/*! @} */
#endif