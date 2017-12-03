////////////////////////////////////////////////////////////////////////////////
// Filename: InterfaceElementClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _INTERFACEELEMENTCLASS_H_
#define _INTERFACEELEMENTCLASS_H_


//////////////
// INCLUDES //
//////////////

#include<string>
#include<codecvt>
#include<fstream>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Bitmapclass.h"
#include "Textclass.h"
#include "InterfaceShaderClass.h"
#include "GlobalManagerClass.h"



////////////////////////////////////////////////////////////////////////////////
// Class name: InterfaceElementClass
////////////////////////////////////////////////////////////////////////////////

/*!
\defgroup InterfaceElements Элементы интерфейса
\ingroup Graphics
@{
*/
/*!
Основной элемент интерфейса
*/
class InterfaceElementClass
{
protected:
	//! Информация о bitmap
	struct BitmapInfo
	{
		BitmapInfo()
		{
			//Create base filename
			filename = PathManagerClass::getI().makePath();
		}

		std::string name; //!<Имя блока
		PathClass* filename; //!<Путь до текстуры
		float width;  //!<Ширина изображения
		float height;  //!<Высота изображения
		float posX;  //!<Позиция по оси Х
		float posY;  //!<Позиция по оси Y

		std::string selAction;  //!< Имя команды, выполняемой при выделении
		std::string unSelAction;  //!< Имя команды, выполняемой при снятии выделения
		std::string pickAction;  //!< Имя команды, выполняемой при клике
		std::string unPickAction;  //!< Имя команды, выполняемой при отклике

		float vertPercent = 1.0f;  //!< Отображаемая часть изображения по вертикали
		float horPercent = 1.0f;  //!< Отображаемоа часть изображения по горизантали
		float selIntensivity = 0;  //!< Яркость выделения
		float transparency = 1.0f;  //!< Прозрачность
		D3DXVECTOR4 selCol = D3DXVECTOR4(1, 1, 1, 1);  //!< Цвет выделения
		bool visible = true;  //!< Состояние отображамости
	};
	//! Информация о строках
	struct StringInfo
	{
		std::string text; //!< Имя текста
		std::string adding = ""; //!< Имя спец добавки после текста
		float maxWidth; //!< Максимальная ширина блока со строкой
		float size;  //!< Размер шрифта
		D3DXVECTOR4 color; //!< Цвет текста
		float posX; //!< Позиция по оси X
		float posY; //!< Позиция по оси Y
	};
	//! Информация о тексте
	struct TextInfo
	{
		//Create base filenames
		TextInfo()
		{
			fontFilename = PathManagerClass::getI().makePath();
			textFilename = PathManagerClass::getI().makePath();
		}
		std::string name; //!< Имя текста
		PathClass* fontFilename; //!< Путь до файла со шрифтом
		PathClass* textFilename; //!< Путь до файла с текстом
		float maxLength; //!< Максимальная длина строки
		int stringsNum; //!< Количество строк
		int orientation; //!< Позиция текста
		StringInfo* strings; //!< Информация о местных строках
		bool visible = true; //!< Состояние отображамости
	};
public:

	InterfaceElementClass();
	InterfaceElementClass(const InterfaceElementClass&);
	~InterfaceElementClass();

	//! Загрузка элемента интерфейса \param[in] device, deviceContext - графическое устройство \param[in] hwnd - идентификатор окна
	//! \param[in] screenWidth, screenHeight - ширина и высота экрана \return false, если были ошибки
	virtual bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, PathClass* filename,
		int screenWidth, int screenHeight);
	virtual void Shutdown();
	//! Прорисовка элемента  интерфейса \param[in] interfaceShader - шейдер интерфейса \param[in] fontShader - шейдер текста \param[in] deviceContext - графическое устройство
	//! \param[in] worldMatrix, viewMatrix, orthoMatrix - матрицы с параметрами \return false, если были ошибки
	virtual bool Render(FontShaderClass* fontShader, InterfaceShaderClass* interfaceShader, ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
		D3DXMATRIX orthoMatrix, D3DXMATRIX baseViewMatrix);

	//Getting element`s data
	//! Получение имени элемента интерфейса \return Имя элемента
	std::string getName(); 
	//! Получение состояния видимости элемента интерфейса \return true, если элемент требует отображения, false, если нет
	bool isVisible();

	//Updating element`s data
	//! Отобразить/скрыть элемент \param[in] visible - 0, если необходимо скрыть, 1, если отобразить
	virtual void setVisible(bool visible);
	//! Обновить положение перетаскиваемых элементов \param[in] deviceContext - графическое устройство \param[in] posX, posY - новая позиция элементов
	virtual void updateDrag(ID3D11DeviceContext* deviceContext, int posX, int posY);
	//! Начать перетаскивание элемента  \param[in] elname - имя элемента \param[in] posX, posY - начальная позиция элементов
	virtual void startDrag( int posX, int posY);
	//! Установить позицию элемента \param[in] deviceContext - графическое устройство  \param[in] elname - имя элемента \param[in] posX, posY - новая позиция элемента
	virtual void setPos(ID3D11DeviceContext* deviceContext, int posX, int posY);
	//! Окончить перетаскивание элемента 
	virtual void endDrag();

	//! Добавление элемента, если это возможно  \param[in] device, deviceContext - графическое устройство 
	//! \param[in] hwnd - идентификатор окна \return false, если были ошибки
	virtual bool addElement(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd);
	//! Удаление элемента, если это возможно   \param[in] deviceContext - графическое устройство \param[in] ind - индекс элемента в списке
	virtual void deleteElement(ID3D11DeviceContext* deviceContext, int ind);
	//! Удаление всех элементов
	virtual void clear();
	//! Получение элемента \param[in] ind - индекс элемента \return подэлемент, если возможно
	virtual InterfaceElementClass* getElement(int ind);

	//Finding index of element by his name
	//! Нахождение индекса bitmap по имени \param[in] name - имя bitmap \return индекс bitmap, 0, если такого нет
	int findBMbyName(const std::string& name);
	//! Нахождение индекса текста по имени \param[in] name - имя текста \return индекс текста, 0, если такого нет
	int findTbyName(const std::string& name);

	//Updating params of bitmaps
	//! Отобразить/скрыть bitmap элемента \param[in] name - имя bitmap \param[in] visible - 0, если необходимо скрыть, 1, если отобразить
	virtual void setBMvisible(const std::string& name, bool visible);
	//! Обновить положение bitmap элемента на X  \param[in] name - имя bitmap \param[in] posX - новая позиция на X
	virtual void updateBMposX(const std::string& name, int posX);
	//! Обновить положение bitmap элемента на Y  \param[in] name - имя bitmap \param[in] posY - новая позиция на Y
	virtual void updateBMposY(const std::string& name, int posY);
	//! Обновить уровень отоброжения bitmap элемента по вертикали  \param[in] name - имя bitmap 
	//! \param[in] vertPercent - новая отображаемая доля (если отрицательная, то удаляется часть с другой стороны)
	virtual void updateBMvertPercent(const std::string& name, float vertPercent);
	//! Обновить уровень отоброжения bitmap элемента по горизонтали  \param[in] name - имя bitmap 
	//! \param[in] horPercent - новая отображаемая доля (если отрицательная, то удаляется часть с другой стороны)
	virtual void updateBMhorPercent(const std::string& name, float horPercent);
	//! Обновить уровень прозрачности bitmap элемента  \param[in] name - имя bitmap 
	//! \param[in] transparency - новый уровень прозрачности
	virtual void updateBMtranspar(const std::string& name, float transparency);
	//! Обновить уровень интенсивности выделения bitmap элемента  \param[in] name - имя bitmap 
	//! \param[in] selIntensivity - новый уровень интенсивности выделения
	virtual void updateBMselIntens(const std::string& name, float selIntensivity);
	//! Обновить цвет выделения bitmap элемента  \param[in] name - имя bitmap 
	//! \param[in] selCol - новый цвет выделения
	virtual void updateBMselCol(const std::string& name, D3DXVECTOR4 selCol);
	//! Обновить текстуру bitmap элемента \param[in] device - графическое устройство  \param[in] name - имя bitmap 
	//!  \param[in] filename - путь до новой текстуры  \return false, если были ошибки
	bool setNewBM(ID3D11Device* device, const std::string& name, PathClass* filename);


	//Updating params of strings
	//! Отобразить/скрыть текста элемента  \param[in] name - имя текста \param[in] visible - 0, если необходимо скрыть, 1, если отобразить
	virtual void setTvisible(const std::string& name, bool visible);
	//! Обновить положение текста элемента на Y  \param[in] name - имя текста  \param[in] ind - индекс строки
	//! \param[in] posY - новая позиция на Y  \return false, если были ошибки
	virtual bool updateTSposY(ID3D11DeviceContext* deviceContext, const std::string& name, int ind, int posY);
	//! Обновить положение текста элемента на X  \param[in] name - имя текста  \param[in] ind - индекс строки
	//! \param[in] posX - новая позиция на X \return false, если были ошибки
	virtual bool updateTSposX(ID3D11DeviceContext* deviceContext, const std::string& name, int ind, int posX);
	//! Обновить содержимое текста элемента  \param[in] name - имя текста  \param[in] ind - индекс строки
	//! \param[in] text - новый текст \return false, если были ошибки
	virtual bool updateTStext(ID3D11DeviceContext* deviceContext, const std::string& name, int ind, const std::string& text);
	//! Обновить цвет текста элемента  \param[in] name - имя текста  \param[in] ind - индекс строки
	//! \param[in] color - новый цвет \return false, если были ошибки
	virtual bool updateTScolor(ID3D11DeviceContext* deviceContext, const std::string& name, int ind, D3DXVECTOR4 color);
	//! Обновить содержимое дополнения к строке текста элемента  \param[in] name - имя текста  \param[in] ind - индекс строки
	//! \param[in] adding - новое дополнение к строке \return false, если были ошибки
	virtual bool updateTSadding(ID3D11DeviceContext* deviceContext, const std::string& name, int ind, const std::string& text);

	//Getting data about strings
	//! Получение длины текста  \param[in] name - имя текста \param[in] ind - индекс строки
	//! return длина строки в тексте, 0, если такой нет
	int getTSsize(const std::string& name, int ind);
	//! Получение текста  \param[in] name - имя текста \param[in] ind - индекс строки
	//! return строка в тексте, "", если такой нет
	std::string getTStext(const std::string& name, int ind);

	//Actions on click
	//! Получить элемент по координатам  \param[in] ind - индекс элемента в классе \param[in] name - имя элемента \return имя bitmap
	virtual  std::string pick(int posX, int posY);
	//! Выполнить выделение bitmap \param[in] name - имя bitmap
	virtual  void addSelCommand(const std::string& name);
	//! Выполнить отмнеу выделения bitmap \param[in] name - имя bitmap
	virtual  void addUnSelCommand(const std::string& name);
	//! Выполнить клик на bitmap \param[in] name - имя bitmap
	virtual  void addPickCommand(const std::string& name);
	//! Выполнить отклик на bitmap \param[in] name - имя bitmap
	virtual  void addUnPickCommand(const std::string& name);

	//! Изменить команду на выделение bitmap \param[in] name - имя bitmap \param[in] action - имя новой команды
	virtual  void setSelCommand(const std::string& name, const std::string& action);
	//! Изменить команду на отмену выделения bitmap \param[in] name - имя bitmap \param[in] action - имя новой команды
	virtual  void setUnSelCommand(const std::string& name, const std::string& action);
	//! Изменить команду на клик на bitmap \param[in] name - имя bitmap \param[in] action - имя новой команды
	virtual  void setPickCommand(const std::string& name, const std::string& action);
	//! Изменить команду на отклик на bitmap \param[in] name - имя bitmap \param[in] action - имя новой команды
	virtual  void setUnPickCommand(const std::string& name, const std::string& action);
	//virtual CommandClass* makeChanges(CommandClass* command, int num);

	//Work with files
	//virtual std::string getTextFromFile(const std::string& name, const std::string& filename);
protected:
	//Work with files
	//! Получить данные об элементе из файла \param[in] filename - путь до файла с данными элемента \return false, если были ошибки
	virtual bool readFromFile(PathClass* filename);
	//! Получить данные о bitmap из файла \param[in] file - открытый файл с данными элемента \return false, если были ошибки
	bool readBitmapsInfoFromFile(std::ifstream* file);
	//! Получить данные о текстах из файла \param[in] file - открытый файл с данными элемента \return false, если были ошибки
	bool readTextsInfoFromFile(std::ifstream* file);
	//! Обновить предложение, на случай возмлжных изменений данных о нем \param[in] deviceContext - графическое устройство 
	//!  \param[in] textInd - индекс текста \param[in] sentenceInd - индекс предложения  \return false, если были ошибки
	bool updateSentence(ID3D11DeviceContext* deviceContext, int textInd, int sentenceInd);
protected:

	int m_screenWidth, m_screenHeight;  //!< Размеры экрана
	std::string m_name;  //!< Имя элемента
	int m_posX, m_posY, m_width, m_height;  //!< Позиция и размеры элемента 

	int m_bitmapsNum;  //!< Количество bitmap
	int m_textsNum;  //!< Количество текстов
	PathClass* m_actionsFilename;  //!< Путь до файла с коммандами
	PathClass* m_fontsFilename;  //!< Путь до файла со шрифтами

	BitmapInfo* m_bitmapsInfo;  //!< Информация о bitmap
	TextInfo* m_textsInfo;  //!< Информация о текстах
	BitmapClass** m_bitmaps;  //!< bitmaps
	TextClass** m_texts;  //!< Тексты

	bool m_vertOrien;  //!< Ориентация элемента по вертикали
	bool m_horOrien;  //!< Ориентация элемента по горизонтали
	bool m_visible = false;  //!< Состояние видимости элемента

	int m_lastX = 0;  //!< Предыдыущее положение элемента при перетаскивании по оси X
	int m_lastY = 0;  //!<  Предыдыущее положение элемента при перетаскивании по оси Y
};
/*! @} */

#endif