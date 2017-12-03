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


/*!
Один класс, чтобы управлять ими всеми
*/
class SystemClass
{
public:
	//!Простой конструктор
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	//!Инициализировать игру \return false, если были ошибки при инициализации, true при успешной инициализации
	bool Initialize();
	//!Вызвать shutdown
	void Shutdown();
	//!Просчитать очередной кадр (выполянется до тех пор, пока не будет инициализирован выход из игры)
	void Run();

	//!базовая callback функция для обработки сообщений (Не знаю, как это работает)
	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

private:
	//!Просчитать кадр \return false, если во время кадра был инициализирован выход
	bool Frame();
	/*!
	\brief Создать окно игры
	\param[in] screenWidth, screenHeight размеры окна
	*/
	void InitializeWindows(int screenWidth, int screenHeight);
	//! Закрыть окно
	void ShutdownWindows();

	//! Выполнить все команды в очереди \return false, если во время выполнения был инициализирован выход
	bool doCommands();
	//! \brief Выполнить только текущую команду \param[in] command исполнемая команда
	//! \return false, если во время выполнения был инициализирован выход
	//! \return false, если во время выполнения был инициализирован выход
	bool doSingleCommand(CommandClass* command);
	//! \brief Выполнить команду, связанную с обновлением состояния системы \param[in] command  исполнемая команда
	//! \param[in] ind индекс команды \param[in] firstCommand индекс первого параметра
	//! \return false, если во время выполнения был инициализирован выход
	bool updateSystem(CommandClass* command, int ind,int firstCommand);
	//! \brief Выполнить команду, связанную с выполнением скриптовых операторов command \param[in] command  исполнемая команда
	//! \param[in] ind индекс команды \param[in] firstCommand индекс первого параметра
	//! \return false, если во время выполнения был инициализирован выход
	bool procesOperators(CommandClass* command, int ind, int firstCommand);
	//! \brief Выполнить команду, дающую некое значение \param[in] command исполнемая команда
	//! \param[in] ind индекс команды \param[in] firstCommand индекс первого параметра
	//! \return false, если во время выполнения был инициализирован выход
	bool get(CommandClass* command, int ind, int firstCommand);
	//! \brief Выполнить команду, устанавливающую некое значение \param[in] command  исполнемая команда
	//! \param[in] ind индекс команды \param[in] firstCommand индекс первого параметра
	//! \return false, если во время выполнения был инициализирован выход
	bool set(CommandClass* command, int ind, int firstCommand);
private:
	LPCWSTR m_applicationName; //!<Название процесса
	HINSTANCE m_hinstance;     //!<hz
	HWND m_hwnd;               //!<hz

	//blocks
	InputClass* m_input;           //!<блок для управления input
	PositionClass* m_position;     //!<блок для управления позицией игрока
	GraphicsClass* m_graphics;     //!<блок для управления графикой
	GameMechanicClass* m_gameMech; //!<блок для управления игровыми механиками

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