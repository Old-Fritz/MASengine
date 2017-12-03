////////////////////////////////////////////////////////////////////////////////
// Filename: SliderElementClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _SLIDERELEMENTCLASS_H_
#define _SLIDERELEMENTCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "InterfaceElementClass.h"



////////////////////////////////////////////////////////////////////////////////
// Class name: SliderElementClass
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup InterfaceElements
@{
*/
/*!
Элемент слайдера
*/
class SliderElementClass : public InterfaceElementClass
{
public:

	SliderElementClass();
	SliderElementClass(const SliderElementClass&);
	~SliderElementClass();

	//! Загрузка элемента интерфейса \param[in] device, deviceContext - графическое устройство \param[in] hwnd - идентификатор окна
	//! \param[in] screenWidth, screenHeight - ширина и высота экрана \return false, если были ошибки
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, PathClass* filename,
		int screenWidth, int screenHeight);


	//sectors changing
	//! Изменить количество секторов \param[in] num - новое количество секторов 
	void updateSectors(int num);
	//! Установить текущий сектор слайдера  \param[in] sector - сектор слайдера
	void setCurrentSector(int sector);
	//! Нахождение сектора слайдера, наиболее близкого к координатам  \param[in] posX, posY - координаты сектора \return индекс сектора
	int findSector(float posX, float posY);
	//! Обновление положения слайдера
	void updateSliderPos();

	//! Установка нового текущего сектора по координатам \param[in] posX, posY - координаты сектора
	void updateSector(float posX, float posY);
	//! Отобразить/скрыть элемент \param[in] visible - 0, если необходимо скрыть, 1, если отобразить
	void setVisible(bool visible);
	//! Обновить положение перетаскиваемых элементов \param[in] deviceContext - графическое устройство \param[in] posX, posY - новая позиция элементов
	 void updateDrag(ID3D11DeviceContext* deviceContext, int posX, int posY);
	 //! Окончить перетаскивание элемента 
	 void endDrag();
	 //! Переход к следующему сектору слайдера
	void nextSector();
	//! Переход к предыдущему сектору слайдера
	void backSector();

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
	  //! Выполнить основную команду слайдера
	void addSliderCommand();
	//CommandClass* makeChanges(CommandClass* command, int num);

private:
	//Work with files
	//! Получить данные об элементе из файла \param[in] filename - путь до файла с данными элемента \return false, если были ошибки
	bool readFromFile(PathClass* filename);

private:
	int m_currentSector; //!< Текущий сектор слайдера
	int m_sectorsNum;; //!< Количество секторов
	int m_orientation;; //!<  Ориентация элемента, 0, если слайдер горизонтальный, 1, если вертикальный
	bool m_dragType = 0; ; //!< Тип перетаскивания. 0 - перетаскивается весь элемент, 1 - только слайдер 
	std::string m_sliderCommand;; //!< Имя основной команды слайдера
};
/*! @} */

#endif