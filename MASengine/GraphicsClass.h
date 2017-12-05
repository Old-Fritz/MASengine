////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "ShaderManagerClass.h"
#include "CameraClass.h"
#include "InterfaceClass.h"
#include "TerrainManagerClass.h"
#include "LightClass.h"
#include "ModelManagerClass.h"

/////////////
// GLOBALS //
/////////////
const float SCREEN_DEPTH = 5000.0f;
const float SCREEN_NEAR = 0.1f;
const int TEST_NUM = 1;

////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////

/*!
\defgroup Graphics Графика
@{
*/
/*!
Класс для управления всей графикой
*/
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	//! Инициализация класса \param[in] hwnd - идентификатор окна \return false, если были ошибки
	bool Initialize(HWND hwnd);
	//! Инициализация графических ресурсов \return false, если были ошибки
	bool InitializeResources();
	void Shutdown();
	void ShutdownResources();
	//! Вычисление кадра \param[in] pos, rot - позиция игрока \param[in] mouseX, mouseY - позиция курсора \return false, если были ошибки
	bool Frame(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int mouseX, int mouseY);

	//! \brief Выполнить команду, связанную с обновлением интерфейса \param[in] command  исполнемая команда
	//! \param[in] ind индекс команды \return false, если во время выполнения был инициализирован выход
	bool updateInterface(CommandClass* command, int ind);
	//! \brief Выполнить команду, связанную с обновлением графики \param[in] command  исполнемая команда
	//! \param[in] ind индекс команды \return false, если во время выполнения был инициализирован выход
	bool updateGraphics(CommandClass* command,int ind);

	
private:
	//! Расчет текущего кара изображения  \return false, если были ошибки
	bool Render();

	// Functions on pick
	//! Выполнение действий при клике мышью  \param[in] mouseX, mouseY - позиция курсора
	void pick(int mouseX, int mouseY);
	//! Выполнение действий при отжатии клика мышью  \param[in] mouseX, mouseY - позиция курсора
	void unPick(int mouseX, int mouseY);
	//! Получение элемента, выбранного при клике \param[in] mouseX, mouseY - позиция курсора
	//! \param[out] ind - индекс выбранного элемента \param[out] name - имя подэлемента
	//! \return false, если не был выбран ни один элемент, true, если был
	bool interfacePick(int mouseX, int mouseY, int& ind, std::string& name);
	//! Получение провинции, выбранной при клике \param[in] mouseX, mouseY - позиция курсора
	//! \param[out] provNum - индекс выбранной провинции \param[out] point - точка пересечения
	//! \return false, если не был выбран ни один элемент, true, если был
	bool terrainPick(int mouseX, int mouseY, int& provNum, D3DXVECTOR3& point);
	//! Создание луча пересечения \param[in] mouseX, mouseY - позиция курсора
	//! \param[out] rayOrigin - основание луча \param[out] rayDirection - направление луча
	void createRay(int mouseX, int mouseY, D3DXVECTOR3& rayOrigin, D3DXVECTOR3& rayDirection);
private:
	//params
	D3DXMATRIX m_baseViewMatrix; //!<Базовая view матрица
	HWND m_hwnd; //!<Идентификатор окна 
	int m_screenWidth, m_screenHeight; //!<Ширина и высота окна

	ModelClass* m_test; //!<Нечто тестовое

	//blocks
	D3DClass* m_D3D; //!<Графическое API
	ShaderManagerClass* m_shaderManager; //!<Менеджер шейдеров
	CameraClass * m_camera; //!<Камера
	LightClass * m_light;  //!<Свет
	InterfaceClass* m_interface; //!<Интерфейс
	TerrainManagerClass* m_terrain; //!<Блоки ландшафта
};
/*! @} */
#endif