////////////////////////////////////////////////////////////////////////////////
// Filename: TextManagerClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _TEXTMANAGERCLASS_H_
#define _TEXTMANAGERCLASS_H_

//////////////
// INCLUDES //
//////////////


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "PathManagerClass.h"
#include <corecrt_wstring.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: TextManagerClass
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup ResourceManagment
@{
*/
/*!
Класс для управления текстами, синглтон
*/
class TextManagerClass
{
public:
	void Shutdown();
	//! \brief Получить экземпляр класса
	static TextManagerClass& getI();

	/*!
	Добавление текста \param[in] name - кодовое имя текста
	\param[in] filename - путь до файла, где хранится полный текст
	*/
	void addText(const std::string& name, PathClass* filename);
	/*!
	Получение текста \param[in] name - кодовое имя текста
	\param[in] filename - путь до файла, где хранится полный текст
	\return полный текст в кодировке UNICODE
	*/
	std::wstring getText(const std::string& name, PathClass* filename);

private:
	TextManagerClass();
	TextManagerClass(const TextManagerClass&);
	~TextManagerClass();

private:
	std::map<long long, std::wstring> m_texts; //!<текста

	static TextManagerClass* m_instance; //!<Единственный экземпляр класса
};
/*! @} */
#endif