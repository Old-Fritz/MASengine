////////////////////////////////////////////////////////////////////////////////
// Filename: LogManagerClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _LOGMANAGERCLASS_H_
#define _LOGMANAGERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <fstream>
#include <string>
#include <ctime>

////////////////////////////////////////////////////////////////////////////////
// Class name: LogManagerClass
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup GlobalManager 
@{
*/
/*!
Класс для управления логами, синглтон
*/
class LogManagerClass
{

public:
	
	/*!
	 \brief Инициализировать менеджер логов \param[in] filepath - путь сохранения логов
	 \details новый файл лога называется LOGММ-ДД-ГГ_ЧЧ-ММ-СС.txt по дате и времени создания
	 \return false, если были ошибки
	*/
	bool Initialize(const std::string& filepath);
	void Shutdown();

	//! \brief Получить экземпляр класса
	static LogManagerClass& getI();

	/*!
	\brief Сделать новую запись в лог-файле \param[in] log - текст записи
	\details Новая запись появляется в новой строке в файле и записывается в следующем формате: 
	ММ-ДД-ГГ_ЧЧ-ММ-СС Текст записи
	*/
	void addLog(const std::string& log);
private:
	LogManagerClass();
	LogManagerClass(const LogManagerClass&);
	~LogManagerClass();

	//! Возвращает текущее дату и время в виде строки в формате ММ-ДД-ГГ_ЧЧ-ММ-СС
	std::string getTime();

private:
	static LogManagerClass* m_instance; //!<Единственный экземпляр класса
	std::ofstream m_file; //!<Файл лога

};
/*! @} */

#endif