////////////////////////////////////////////////////////////////////////////////
// Filename: GroupElementClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _GROUPELEMENTCLASS_H_
#define _GROUPELEMENTCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "InterfaceElementClass.h"
#include "ListElementClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: GroupElementClass
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup InterfaceElements
@{
*/
/*!
Элемент группы элементов
*/
class GroupElementClass : public InterfaceElementClass
{
public:
	GroupElementClass();
	GroupElementClass(const GroupElementClass&);
	~GroupElementClass();

	//! Загрузка элемента интерфейса \param[in] device, deviceContext - графическое устройство \param[in] hwnd - идентификатор окна
	//! \param[in] screenWidth, screenHeight - ширина и высота экрана \return false, если были ошибки
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, PathClass* filename,
		int screenWidth, int screenHeight);
	void Shutdown();
	//! Прорисовка элемента  интерфейса \param[in] interfaceShader - шейдер интерфейса \param[in] fontShader - шейдер текста \param[in] deviceContext - графическое устройство
	//! \param[in] worldMatrix, viewMatrix, orthoMatrix - матрицы с параметрами \return false, если были ошибки
	 bool Render(FontShaderClass* fontShader, InterfaceShaderClass* interfaceShader, ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
		D3DXMATRIX orthoMatrix, D3DXMATRIX baseViewMatrix);

	//Elements changing
	//! Отобразить/скрыть элемент \param[in] visible - 0, если необходимо скрыть, 1, если отобразить
	  void setVisible(bool visible);
	 //! Обновить положение перетаскиваемых элементов \param[in] deviceContext - графическое устройство \param[in] posX, posY - новая позиция элементов
	  void updateDrag(ID3D11DeviceContext* deviceContext, int posX, int posY);
	 //! Начать перетаскивание элемента  \param[in] elname - имя элемента \param[in] posX, posY - начальная позиция элементов
	  void startDrag(int posX, int posY);
	 //! Окончить перетаскивание элемента 
	void endDrag();

	//! Добавление элемента, если это возможно  \param[in] device, deviceContext - графическое устройство 
	//! \param[in] hwnd - идентификатор окна \return false, если были ошибки
	 bool addElement(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd);
	//! Удаление элемента, если это возможно   \param[in] deviceContext - графическое устройство \param[in] ind - индекс элемента в списке
	 void deleteElement(int ind);
	//! Удаление всех элементов
	virtual void clear();

	//Actions on click
	//! Получить элемент по координатам  \param[in] ind - индекс элемента в классе \param[in] name - имя элемента \return имя bitmap
	  std::string pick(int posX, int posY);
	//! Выполнить выделение bitmap \param[in] name - имя bitmap
	  void addSelCommand(const std::string& name);
	//! Выполнить отмнеу выделения bitmap \param[in] name - имя bitmap
	  void addUnSelCommand(const std::string& name);
	//! Выполнить клик на bitmap \param[in] name - имя bitmap
	  void addPickCommand(const std::string& name);
	//! Выполнить отклик на bitmap \param[in] name - имя bitmap
	  void addUnPickCommand(const std::string& name);

	//CommandClass* makeChanges(CommandClass* command, int num);

	//get elements
	//! Получение элемента \param[in] ind - индекс элемента \return подэлемент, если возможно
	 InterfaceElementClass* getElement(int ind);

private:
	//Work with files
	//! Получить данные об элементе из файла \param[in] filename - путь до файла с данными элемента \return false, если были ошибки
	bool readFromFile(PathClass* filename);
	//! \brief Получение номера элемента в списке из имени   \param[in] name - имя элемента \return номер элемента
	//! \details из строки вида NUM_NAME получает NUM и вычисляет его
	int getNumFromName(const std::string& name);

private:
	int m_length = 0; //!< Количество элементов
	std::string m_elType;  //!< Тип элементов
	PathClass* m_elementFilename;  //!< Путь до файла с элементом интерфейса
	std::vector<InterfaceElementClass*> m_elements;  //!< Подэлементы
};
/*! @} */
#endif