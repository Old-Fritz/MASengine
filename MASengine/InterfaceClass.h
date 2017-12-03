////////////////////////////////////////////////////////////////////////////////
// Filename: interfaceclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INTERFACECLASS_H_
#define _INTERFACECLASS_H_

/////////////
// GLOBALS //
/////////////


//////////////
// INCLUDES //
//////////////

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "InterfaceShaderClass.h"
#include "GroupElementClass.h"
#include "FontShaderClass.h"
#include "LoadScreenManagerClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: InterfaceClass
////////////////////////////////////////////////////////////////////////////////

/*!
\defgroup Interface Интерфейс
\ingroup Graphics
@{
*/
/*!
Класс для управления всем интерфейсом
*/
class InterfaceClass
{
public:
	InterfaceClass();
	InterfaceClass(const InterfaceClass&);
	~InterfaceClass();

	//bool baseInitialize(ID3D10Device* device, HWND hwnd, int screenWidth, int screenHeight, D3DXMATRIX BaseViewMatrix,
	//	ResourceManagerClass* resources);
	//bool elementsInitialize(ID3D10Device* device, HWND hwnd, int screenWidth, int screenHeight,
	//	D3DXMATRIX BaseViewMatrix, D3DClass* D3D);

	//! Загрузка интерфейса \param[in] device, deviceContext - графическое устройство \param[in] hwnd - идентификатор окна
	//! \param[in] screenWidth, screenHeight - ширина и высота экрана \return false, если были ошибки
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight);
	void Shutdown();
	//! Расчет покадровых измений интерфейса \param[in] deviceContext - графическое устройство \param[in] mouseX, mouseY - положение курсора \return false, если были ошибки
	bool Frame(ID3D11DeviceContext* deviceContext, int mouseX, int mouseY);
	//! Прорисовка интерфейса \param[in] interfaceShader - шейдер интерфейса \param[in] fontShader - шейдер текста \param[in] deviceContext - графическое устройство
	//! \param[in] worldMatrix, viewMatrix, orthoMatrix - матрицы с параметрами \return false, если были ошибки
	bool Render(InterfaceShaderClass* interfaceShader, FontShaderClass* fontShader,
		ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX orthoMatrix);

	//update data of interface
	//! Отобразить/скрыть элемент \param[in] elname - имя элемента \param[in] visible - 0, если необходимо скрыть, 1, если отобразить
	void setElVisible(const std::string& elname, bool visible);
	//void updateLoadScreen(string loadText, float loadPercent, D3DClass* D3D);
	//! Обновить положение перетаскиваемых элементов \param[in] deviceContext - графическое устройство \param[in] posX, posY - новая позиция элементов
	void updateDrag(ID3D11DeviceContext* deviceContext, int posX, int posY);
	//! Начать перетаскивание элемента  \param[in] elname - имя элемента
	void startDrag(const std::string& elname);
	//! Окончить перетаскивание элемента  \param[in] elname - имя элемента
	void endDrag(const std::string& elname);
	//! Установить позицию элемента \param[in] deviceContext - графическое устройство  \param[in] elname - имя элемента \param[in] posX, posY - новая позиция элемента
	void setPos(ID3D11DeviceContext* deviceContext, const std::string& elname, int posX, int posY);

	//Updating params of bitmaps
	//! Отобразить/скрыть bitmap элемента \param[in] elname - имя элемента \param[in] bmname - имя bitmap \param[in] visible - 0, если необходимо скрыть, 1, если отобразить
	void setElBMvisible(const std::string& elname, const std::string& bmname, bool visible);
	//! Обновить положение bitmap элемента на X \param[in] elname - имя элемента \param[in] bmname - имя bitmap \param[in] posX - новая позиция на X
	void updateElBMposX(const std::string& elname, const std::string& bmname, int posX);
	//! Обновить положение bitmap элемента на Y \param[in] elname - имя элемента \param[in] bmname - имя bitmap \param[in] posY - новая позиция на Y
	void updateElBMposY(const std::string& elname, const std::string& bmname, int posY);
	//! Обновить уровень отоброжения bitmap элемента по вертикали \param[in] elname - имя элемента \param[in] bmname - имя bitmap 
	//! \param[in] vertPercent - новая отображаемая доля (если отрицательная, то удаляется часть с другой стороны)
	void updateElBMvertPercent(const std::string& elname, const std::string& bmname, float vertPercent);
	//! Обновить уровень отоброжения bitmap элемента по горизонтали \param[in] elname - имя элемента \param[in] bmname - имя bitmap 
	//! \param[in] horPercent - новая отображаемая доля (если отрицательная, то удаляется часть с другой стороны)
	void updateElBMhorPercent(const std::string& elname, const std::string& bmname, float horPercent);
	//! Обновить уровень прозрачности bitmap элемента \param[in] elname - имя элемента \param[in] bmname - имя bitmap 
	//! \param[in] transparency - новый уровень прозрачности
	void updateElBMtranspar(const std::string& elname, const std::string& bmname, float transparency);
	//! Обновить уровень интенсивности выделения bitmap элемента \param[in] elname - имя элемента \param[in] bmname - имя bitmap 
	//! \param[in] selIntensivity - новый уровень интенсивности выделения
	void updateElBMselIntens(const std::string& elname, const std::string& bmname, float selIntensivity);
	//! Обновить цвет выделения bitmap элемента \param[in] elname - имя элемента \param[in] bmname - имя bitmap 
	//! \param[in] selCol - новый цвет выделения
	void updateElBMselCol(const std::string& elname, const std::string& bmname, D3DXVECTOR4 selCol);
	//! Обновить команду при выборе bitmap элемента \param[in] elname - имя элемента \param[in] bmname - имя bitmap 
	//!  \param[in] commandType - тип команды (pick,unPick,sel,unSel)  \param[in] command - имя новой команды (берется из того же файла, что и старая)
	void setBMCommand(const std::string& elname, const std::string& bmname, const std::string& commandType, const std::string& command);
	//! Обновить текстуру bitmap элемента \param[in] device - графическое устройство \param[in] elname - имя элемента \param[in] bmname - имя bitmap 
	//!  \param[in] filename - путь до новой текстуры  \return false, если были ошибки
	bool setNewBM(ID3D11Device* device, const std::string& elname, const std::string& bmname, PathClass* filename);

	//Updating params of strings
	//! Отобразить/скрыть текста элемента \param[in] elname - имя элемента \param[in] tname - имя текста \param[in] visible - 0, если необходимо скрыть, 1, если отобразить
	void setElTvisible(const std::string& elname, const std::string& tname, bool visible);
	//! Обновить положение текста элемента на Y \param[in] elname - имя элемента \param[in] tname - имя текста  \param[in] ind - индекс строки
	//! \param[in] posY - новая позиция на Y  \return false, если были ошибки
	bool updateElTSposY(ID3D11DeviceContext* deviceContext, const std::string& elname, const std::string& tname, int ind, int posY);
	//! Обновить положение текста элемента на X \param[in] elname - имя элемента \param[in] tname - имя текста  \param[in] ind - индекс строки
	//! \param[in] posX - новая позиция на X \return false, если были ошибки
	bool updateElTSposX(ID3D11DeviceContext* deviceContext, const std::string& elname, const std::string& tname, int ind, int posX);
	//! Обновить содержимое текста элемента \param[in] elname - имя элемента \param[in] tname - имя текста  \param[in] ind - индекс строки
	//! \param[in] text - новый текст \return false, если были ошибки
	bool updateElTStext(ID3D11DeviceContext* deviceContext, const std::string& elname, const std::string& tname, int ind, const std::string& text);
	//! Обновить цвет текста элемента \param[in] elname - имя элемента \param[in] tname - имя текста  \param[in] ind - индекс строки
	//! \param[in] color - новый цвет \return false, если были ошибки
	bool updateElTScolor(ID3D11DeviceContext* deviceContext, const std::string& elname, const std::string& tname, int ind, D3DXVECTOR4 color);
	//! Обновить содержимое дополнения к строке текста элемента \param[in] elname - имя элемента \param[in] tname - имя текста  \param[in] ind - индекс строки
	//! \param[in] adding - новое дополнение к строке \return false, если были ошибки
	bool updateElTSadding(ID3D11DeviceContext* deviceContext, const std::string& elname, const std::string& tname, int ind, const std::string& adding);

	//Special Slider commands
	//! Перемещение сектора слайдера вперед \param[in] elname - имя элемента слайдера
	void nextSector(const std::string& elname);
	//! Перемещение сектора слайдера назад \param[in] elname - имя элемента слайдера
	void backSector(const std::string& elname);

	//Special List commands
	//! Добавление нового базового элемента в список  \param[in] device, deviceContext - графическое устройство 
	//! \param[in] hwnd - идентификатор окна \param[in] elname - имя элемента списка \return false, если были ошибки
	bool addElement(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, const std::string& elname);
	//! Удаление элемента из списка  \param[in] deviceContext - графическое устройство 
	//!  \param[in] elname - имя элемента списка \param[in] ind - индекс элемента в списке
	void deleteElement(ID3D11DeviceContext* deviceContext, const std::string& elname, int ind);
	//! Установка последнего отображаемого элемента в списке  \param[in] deviceContext - графическое устройство 
	//!  \param[in] elname - имя элемента списка \param[in] ind - индекс элемента в списке
	void setLastElement(ID3D11DeviceContext * deviceContext, const std::string& elname, int ind);
	//! Полная очистка списка \param[in] elname - имя элемента списка
	void clear(const std::string& elname);

	//Special settings command
	//CommandClass* getSettings(string elname, string tname, int ind, string settingsType, string filename, ID3D10Device* device, HWND hwnd);

	//Finding index of element by his name
	//! Нахождение ссылки на элемент по его имени \param[in] elname - имя элемента \return найденный элемент (NULL при отсутствии)
	InterfaceElementClass* findElbyName(const std::string& name);
	//CommandClass* makeChanges(CommandClass* command, string name);

	//Getting data about elements 
	//! Получение элемента, находящегося под определенными координатами \param[in] posX, posY - координаты элемента 
	//! \param[out] ind - индекс элемента в классе    \param[out] name - имя элемента
	//! \return true, если был найден какой-нибудь эл-т, false, если не был
	bool getEl(int posX, int posY, int& ind, std::string& name);
	//! Получение длины текста  \param[in] elname - имя элемента  \param[in] tname - имя текста \param[in] ind - индекс строки
	//! return длина строки в тексте, 0, если такой нет
	int getElTSsize(const std::string& elname, const std::string& tname, int ind);
	//! Получение статуса отображаемости элемента  \param[in] elname - имя элемента \return true, если элемент отображается, false, если не отображается
	bool isElVisible(const std::string& elname);

	//! Обновление дебажного окна \param[in] deviceContext - графическое устройство  \param[in] pos - позиция камеры
	//! \param[in] rot - поворот камеры \param[in] mouseX, mouseY - положение курсора
	void UpdateDebug(ID3D11DeviceContext* deviceContext, D3DXVECTOR3 pos, D3DXVECTOR3 rot, int mouseX, int mouseY);

	//Actions on pick and unpick
	//! Выполнить действия при нажатии на элемент  \param[in] ind - индекс элемента в классе    \param[in] name - имя элемента
	void pick(int ind, const std::string& name);
	//! Выполнить действия при отжатии элемента  \param[in] ind - индекс элемента в классе    \param[in] name - имя элемента
	void unPick(int ind, const std::string& name);
	//! \brief Получение номера элемента в списке из имени   \param[in] name - имя элемента \return номер элемента
	//! \details из строки вида NUM_NAME получает NUM и вычисляет его
	int getNumFromName(const std::string& name);
private:
	//! Нахождение элемента под курсором и выполнение действий, связанных с его выделением \param[in] deviceContext - графическое устройство  \param[in] mouseX, mouseY - положение курсора
	void CheckSelection(ID3D11DeviceContext* deviceContext, int mouseX, int mouseY);
private:
	std::vector<std::string> m_dragElements; //!<Имена текущих перетаскиваемых элементов

	int m_screenWidth, m_screenHeight; //!<Разрешение экрана
	int m_mouseX, m_mouseY; //!<Положение курсора
	int m_elementsNum; //!<Количество элементов

	InterfaceElementClass** m_interfaceElements; //!<Элементы интерфейса

	int m_lastInd = 0; //!<Индекс последнего выделяемого элемента
	std::string m_lastName = ""; //! Имя последнего выделяемого подэлемента

	bool m_isUnSel = true; //!<Текущее состояние выделения
	bool m_LBMDown = false; //!<Состояние ЛКМ
	
	CalculatorClass* m_calculator; //!<Калькулятор
};
/*! @} */
#endif