////////////////////////////////////////////////////////////////////////////////
// Filename: SettingsClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SETTINGSCLASS_H_
#define _SETTINGSCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <filesystem>
#include<string>
#include<vector>
#include<fstream>
#include <map>
#include "MemoryManagerClass.h"
#include "PathManagerClass.h"
#include "PathParamClass.h"
#include "StrParamClass.h"
#include "FloatParamClass.h"
#include "IntParamClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: SettingsClass
////////////////////////////////////////////////////////////////////////////////

/*!
\defgroup Settings Управление настройками
\ingroup GlobalManager
@{
*/
/*!
Класс для управления настройками, синглтон
*/
class SettingsClass
{
public:

	//! Инициализация класса \param[in] filename - путь до файла с данными по инициализации настроек \return false, если были ошибки
	bool Initialize(PathClass* filename);
	//! Сохранение настроек
	void save();
	void Shutdown();

	//! \brief Получить экземпляр класса
	static SettingsClass& getI();

	//! Получение парметра типа int \param[in] name - имя параметра \return Параметр в виде целого числа
	int getIntParameter(const std::string& name);
	//! Получение парметра типа float \param[in] name - имя параметра \return Параметр в виде числа с плавающей запятой
	float getFloatParameter(const std::string& name);
	//! Получение парметра типа string \param[in] name - имя параметра \return Параметр в виде строки
	std::string getStrParameter(const std::string& name);
	//! Получение парметра типа PathClass \param[in] name - имя параметра \return Параметр в виде путя до файла
	PathClass* getPathParameter(const std::string& name);

	//! Установка парметра типа int \param[in] name - имя параметра \param[in] value - параметр в виде целого числа
	void setIntParameter(const std::string& name, int value);
	//! Установка парметра типа float \param[in] name - имя параметра \param[in] value - параметр в виде числа с плавающей запятой
	void setFloatParameter(const std::string& name, float value);
	//! Установка парметра типа string \param[in] name - имя параметра \param[in] value - параметр в виде строки
	void setStrParameter(const std::string& name, const std::string& value);
	//! Установка парметра типа PathClass \param[in] name - имя параметра \param[in] value - параметр в виде путя до файла
	void setPathParameter(const std::string& name, PathClass* value);

private:
	SettingsClass();
	SettingsClass(const SettingsClass&);
	~SettingsClass();
	bool readFromFile(PathClass* filename);
private:
	PathClass* m_filename; //!<Путь к файлу с пользовательскими настройками (сохранение происходит туда)
	std::map<int, SetParamClass*> m_parameters; //!<Различные параметры


	static SettingsClass* m_instance; //!<Единственный экземпляр класса
};
/*! @} */
#endif