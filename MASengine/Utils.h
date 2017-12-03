////////////////////////////////////////////////////////////////////////////////
// Filename: UtilsClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _UTILSCLASS_H_
#define _UTILSCLASS_H_

#include <string>
#include <filesystem>
#include <corecrt_wstring.h>
#include <fstream>

////////////////////////////////////////////////////////////////////////////////
// Class name: UtilsClass
////////////////////////////////////////////////////////////////////////////////

/*!
\defgroup Utils Различные утилиты
@{
*/
/*!
Некоторые общие статические функции
*/
class Utils
{
public:
	//! Получить хэш от строки \param[in] str - строка, от которой берется хэш \return Хэш от строки
	static int getHash(const std::string & str);

	//! Получение текста из файла \param[in] name - имя текста \param[in] filename - имя файла с текстом \return Текст из файла или name, если его не оказалось
	static std::string getTextFromFile(const std::string& name, const std::string& filename);
	//! Получение текста из файла \param[in] name - имя текста \param[in] filename - имя файла с текстом \return Текст из файла в UNICODE или name, если его не оказалось
	static std::wstring getWTextFromFile(const std::string& name, const std::string&  filename);

	//! Конвертация обычной строки в UNICODE \param[in] str - конвертируемая строка \return Результат конвертации
	static std::wstring from_bytes(const std::string& str);
	//! Конвертация UNICODE строки в обычную \param[in] str - конвертируемая строка \return Результат конвертации
	static std::string to_bytes(const std::wstring& str);

	//! Вычисление расстояния между точками \param[in] p1X,p1Y,p1Z - координаты первой точки \param[in] p2X,p2Y,p2Z - координаты второй точки \return Расстояние между точками
	static float calcDist(float p1X, float p1Y, float p1Z, float p2X, float p2Y, float p2Z);
private:
	Utils();

private:
	static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> m_converter; //!<Конвертор строк
};
/*! @} */

#endif