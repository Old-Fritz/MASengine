////////////////////////////////////////////////////////////////////////////////
// Filename: fpsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _FPSCLASS_H_
#define _FPSCLASS_H_


/////////////
// LINKING //
/////////////
#pragma comment(lib, "winmm.lib")


//////////////
// INCLUDES //
//////////////
#include <windows.h>
#include <mmsystem.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: FpsClass
////////////////////////////////////////////////////////////////////////////////
/*!
\addtogroup SystemStateManagment
@{
*/
/*!
Класс для получения значений fps
*/
class FpsClass
{
public:
	FpsClass();
	FpsClass(const FpsClass&);
	~FpsClass();

	//! Инициализировать класс
	void Initialize();
	//! Посчитать время между кадрами, выполняется на каждом кадре
	void Frame();
	//! Получить fps \return значение fps
	int GetFps();

private:
	int m_fps, m_count;
	unsigned long m_startTime;
};
/*! @} */
#endif