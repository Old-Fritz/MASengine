////////////////////////////////////////////////////////////////////////////////
// Filename: LoadManagerClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _LOADSCREENMANAGERCLASS_H_
#define _LOADSCREENMANAGERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include "D3DClass.h"
#include "GM.h"
#include "InterfaceElementClass.h"
#include "ShaderManagerClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: LoadManagerClass
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup Graphics
@{
*/
/*!
Управление загрузочным экраном, синглтон
*/
class LoadScreenManagerClass
{

public:
	//! Инициализация класса \param[in] D3D - графическое API \param[in] shaders - шейдеры \param[in] hwnd - идентификатр окна
	//! \param[in] filename - путь до файла с данными о загрузке \return false, если были ошибки
	bool Initialize(D3DClass* D3D, ShaderManagerClass* shaders, D3DXMATRIX baseViewMatrix, HWND hwnd, PathClass*  filename);
	void Shutdown();
	//! Отрисовка кадра  \return false, если были ошибки
	bool Render();

	//! \brief Получить экземпляр класса
	static LoadScreenManagerClass& getI();

	//! Изменение загрузочной линии \param[in] text - новый текст для линии  \param[in] perCent - процент заполнения линии 
	void changeLine(const std::string& text, float perCent);
	//! Показать загрузочный экран
	void showElements();
	//! Скрыть загрузочный экран
	void hideElements();
private:
	LoadScreenManagerClass();
	LoadScreenManagerClass(const LoadScreenManagerClass&);
	~LoadScreenManagerClass();

	//! Прочитать данные из файла \param[in] filename - путь до файла с данными о загрузке \return false, если были ошибки
	bool readFromFile(PathClass*  filename);

	//! Проверить необходимость смены картинки и цитаты \return false, если были ошибки
	bool updateElements();
	

	//line element
	//! Изменить уровень заполненности линии \param[in] perCent - процент заполнения линии 
	void changeLinePosition(float perCent);
	//! Изменить текст линии \param[in] text - новый текст для линии
	bool changeLineText(const std::string& text);

	//image element
	//! Изменить картинку (рандом или по порядку) \return false, если были ошибки
	bool changeImage();

	//quote element
	//! Изменить цитату (рандом или по порядку) \return false, если были ошибки
	bool changeQuote();
private:
	static LoadScreenManagerClass* m_instance; //!<Единственный экземпляр класса
	long long int m_lastTime; //! Время последнего изменения линии

	//graphics parts
	D3DXMATRIX m_baseViewMatrix;  //!< Базовая видовая матрица
	ShaderManagerClass* m_shaders; //!<Шейдеры
	D3DClass* m_D3D; //!<Графическое API

	//elements
	InterfaceElementClass* m_loadImage; //!<Загрузочная каритнка
	InterfaceElementClass* m_loadLine; //!<Загрузочная линия
	InterfaceElementClass* m_loadQuote; //!<Загрузочная цитата
	std::vector<InterfaceElementClass*> m_additionalElements;  //!<Дополнительные элементы

	//Image parts
	PathClass*  m_imageElementName;  //!<Путь до элемента загрузочной картинки
	std::vector<PathClass*> m_loadImageNames;  //!<Пути до текстур для загрузочной картинки
	bool m_imageChangeRand;  //!<Тип смены картинки (1 - рандом, 0 - порядок)
	int m_imageChangeInterval;  //!<Интервал смены картинки
	int m_imageCurrentInterval;  //!<Время с последнего изменения картинки
	int m_currentImage;  //!<Индекс текущей картинки

	//Quote parts
	PathClass*  m_quoteElementName; //!<Путь до элемента загрузочной цитаты
	std::vector<std::string> m_loadQuoteNames; //!<Имена текстов для загрузочной цитаты
	bool m_quoteChangeRand; //!<Тип смены цитаты (1 - рандом, 0 - порядок)
	int m_quoteChangeInterval; //!<Интервал смены цитаты
	int m_quoteCurrentInterval; //!<Время с последнего изменения цитаты
	int m_currentQuote; //!<Индекс текущей цитаты

	//Line parts
	PathClass*  m_lineElementName; //!<Путь до элемента линии

	//Additional elements parts
	std::vector<PathClass*> m_additionalElementNames; //!<Пути до дополнительных элементов

};
/*! @} */
#endif