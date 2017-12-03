////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_


///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define DIRECTINPUT_VERSION 0x0800

/////////////
// LINKING //
/////////////
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


//////////////
// INCLUDES //
//////////////
#include <dinput.h>
#include "GlobalManagerClass.h"



////////////////////////////////////////////////////////////////////////////////
// Class name: InputClass
////////////////////////////////////////////////////////////////////////////////


/*!
Обработка ввода
*/
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();
	//! Инициализация класса \param[in] hinstance - текущий процесс  \param[in] hwnd - текущее окно \return false, если были ошибки
	bool Initialize(HINSTANCE hinstance, HWND hwnd);
	void Shutdown();
	//! Покадровое считывание данных с устройств ввода \return false, если были ошибки
	bool Frame();

	//! Проверка на нажатие esc \return false, если esc не нажата, true, если нажата
	bool IsEscapePressed();
	//! Получение позиции курсора \param[out] mouseX, mouseY - позиция курсора
	void GetMouseLocation(int& mouseX, int& mouseY);
	//! Получение состояния клавиатуры \return массив символов, размер которого равен числу кнопок. Ели символ - 0, то кнопка не нажата, иначе если 1, то нажата
	unsigned char* GetKeyboardState();

	//set new commands
	//! Изменение команды, выполняемой при нажатии на кнопку \param[in] butNum - номер кнопки  \param[in] command - имя новой команды
	void setPickCommand(int butNum, const std::string& command);
	//! Изменение команды, выполняемой при отпускании кнопки \param[in] butNum - номер кнопки  \param[in] command - имя новой команды
	void setUnPickCommand(int butNum, const std::string& command);
	//! Изменение команды, выполняемой при повороте колеса мыши вверх \param[in] command - имя новой команды
	void setUpWheelCommand(const std::string& command);
	//! Изменение команды, выполняемой при повороте колеса мыши вниз \param[in] command - имя новой команды
	void setDownWheelCommand(const std::string& command);

	//check buttons
	//! Проверка на нажатие лкм \return false, если лкм не нажата, true, если нажата
	bool IsLeftMouseButtonDown();
	//! Проверка на нажатие скм \return false, если скм не нажата, true, если нажата
	bool IsMiddleMouseButtonDown();
private:
	//! Прочитать данные с клавиатуры \return false, если были ошибки
	bool ReadKeyboard();
	//! Прочитать данные с мыши \return false, если были ошибки
	bool ReadMouse();
	//! Расчитать позицию курсора
	void ProcessInput();

private:
	IDirectInput8* m_directInput; //!<Класс ввода от directx
	IDirectInputDevice8* m_keyboard; //!<Класс управления клавиатурой от directx
	IDirectInputDevice8* m_mouse; //!<Класс управления мышью от directx

	unsigned char m_keyboardState[256]; //!<Текущее состояние клавиатуры
	unsigned char m_lastKeyboardState[256]; //!<Прошлое состояние клавиатуры
	DIMOUSESTATE m_lastMouseState; //!Прошлое состояние мыши
	DIMOUSESTATE m_mouseState; //!<Текущее состояние мыши

	std::string m_pickCommandsNames[259]; //!<Имена комманд для нажатия для каждой кнопки
	std::string m_unPickCommandsNames[259]; //!<Имена комманд для отпуска для каждой кнопки
	std::string m_upWheelCommand; //!<Комманда для прокрутки колеса мыши вверх
	std::string m_downWheelCommand; //!<Комманда для прокрутки колеса мыши вниз
	PathClass* m_commandsFileName;  //!<Файл с кодом комманд

	int m_mouseX, m_mouseY; //!<Положение курсора
};

#endif