////////////////////////////////////////////////////////////////////////////////
// Filename: LogManager.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _LogManager_H_
#define _LogManager_H_


//////////////
// INCLUDES //
//////////////
#include <fstream>
#include <string>
#include <ctime>

////////////////////////////////////////////////////////////////////////////////
// Class name: LogManager
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup GlobalManager 
@{
*/
/*!
Класс для управления логами, синглтон
*/
class LogManager
{

public:
	LogManager();
	LogManager(const LogManager&);
	~LogManager();
	/*!
	 \brief Инициализировать менеджер логов \param[in] filepath - путь сохранения логов
	 \details новый файл лога называется LOGММ-ДД-ГГ_ЧЧ-ММ-СС.txt по дате и времени создания
	 \return false, если были ошибки
	*/
	bool Initialize(const std::string& filepath);
	void Shutdown();

	/*!
	\brief Сделать новую запись в лог-файле \param[in] log - текст записи
	\details Новая запись появляется в новой строке в файле и записывается в следующем формате: 
	ММ-ДД-ГГ_ЧЧ-ММ-СС Текст записи
	*/
	void addLog(const std::string& log);
private:
	

	//! Возвращает текущее дату и время в виде строки в формате ММ-ДД-ГГ_ЧЧ-ММ-СС
	std::string getTime();

private:
	std::ofstream m_file; //!<Файл лога

};
/*! @} */

#endif