////////////////////////////////////////////////////////////////////////////////
// Filename: FontManagerClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _FONTMANAGERCLASS_H_
#define _FONTMANAGERCLASS_H_

//////////////
// INCLUDES //
//////////////


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "FontClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: FontManagerClass
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup ResourceManagment
@{
*/
/*!
Класс для управления шрифтами, синглтон
*/
class FontManagerClass
{
public:
	void Shutdown();
	//! \brief Получить экземпляр класса
	static FontManagerClass& getI();

	/*!
	Добавление шрифта \param[in] device - графическое устройство
	\param[in] filename - путь до шрифта
	\return false, если были ошибки
	*/
	bool addFont(ID3D11Device* device, PathClass* filename);
	//! Получение шрифта \param[in] filename - путь до текстуры \return шрифт
	FontClass* getFont(PathClass* filename);
	//! см. getFont, но по хэшу пути
	FontClass* getFont(int hash);
private:
	FontManagerClass();
	FontManagerClass(const FontManagerClass&);
	~FontManagerClass();
private:
	std::map<int, FontClass*> m_fonts; //!<Шрифты

	static FontManagerClass* m_instance; //!<Единственный экземпляр класса
};
/*! @} */
#endif