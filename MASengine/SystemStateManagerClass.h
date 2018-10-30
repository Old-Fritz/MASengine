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
#include "MemoryManager.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: SystemStateManagerClass
////////////////////////////////////////////////////////////////////////////////

/*!
\defgroup SystemStateManagment Управление состоянием системы
\ingroup GlobalManager
@{
*/
/*!
Класс для управления модулями сведений о системе, синглтон
*/
class SystemStateManagerClass
{
public:
	
	//! Инициализировать класс \return false, если были ошибки
	bool Initialize();
	//! Обновить данные, выполняется на каждом кадре
	void Frame();
	void Shutdown();

	//! \brief Получить экземпляр класса
	static SystemStateManagerClass& getI();

	//! Получить таймер \return таймер
	TimerClass* getTimer();
	//! Получить загруженность процессора \return число от 0 до 100 - загруженность процессора в процентах
	int GetCpuPercentage();
	//! Получить fps \return значение fps
	int GetFps();
private:
	SystemStateManagerClass();
	SystemStateManagerClass(const SystemStateManagerClass&);
	~SystemStateManagerClass();
private:
	CpuClass* m_cpu; //!< блок состояния ЦП
	FpsClass* m_fps; //!< блок состояния fps
	TimerClass* m_timer; //!< блок состояния межкадрового времени

	static SystemStateManagerClass* m_instance; //!<Единственный экземпляр класса
};
/*! @} */
#endif