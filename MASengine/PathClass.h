////////////////////////////////////////////////////////////////////////////////
// Filename: PathClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _PATHCLASS_H_
#define _PATHCLASS_H_

#include <string>
#include "Utils.h"
#include "ModManagerClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: PathClass
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup Utils
@{
*/
/*!
Путь до файла
*/
class PathClass
{
public:
	PathClass();
	//! Создает путь по строке \param[in] filename - путь до файла
	PathClass(const std::string& filename);
	//! Создает путь по строке в UNICODE \param[in] filename - путь до файла в UNICODE 
	PathClass(const std::wstring& filename);
	PathClass(const PathClass&);
	~PathClass();

	void Shutdown();

	//! Получение пути со всеми модификациями \return путь до файла в виде строки
	std::string getPath();
	//! Получение пути со всеми модификациями \return путь до файла в виде строки в UNICODE
	std::wstring getWPath();

	//! Получение пути  без учета модификаций \return путь до файла в виде строки
	std::string getShortPath();
	//! Получение пути  без учета модификаций \return путь до файла в виде строки в UNICODE
	std::wstring getShortWPath();

	//! Изменение пути (модификации меняются автоматически) \param[in] filename - путь до файла
	void changePath(const std::string& filename);
	//! Изменение пути (модификации меняются автоматически) в UNICODE \param[in] filename - путь до файла в UNICODE
	void changePath(const std::wstring& filename);

	//! Ручное изменение модификаций \param[in] prefix - модификации, вставляемые перед оригинальным именем файла
	void changePrefix(const std::string& prefix);
	//! Ручное изменение модификаций в UNICODE \param[in] prefix - модификации, вставляемые перед оригинальным именем файла в UNICODE
	void changePrefix(const std::wstring& prefix);

	//! Получение хэша от всего пути \return хэш только оригинального пути
	int getHash();

	//! Получение расширения файла (расширение должно быть выделено точкой последней) \return расширение файла без '.'
	std::string getExpansion();
private:
	std::string m_path; //!<Оригинальный путь
	std::string m_prefix; //!<Специальная модификация, которая меняет путь до файла в зависимости от модов
};

std::ifstream &operator >> (std::ifstream &in, PathClass* path);
/*! @} */
#endif