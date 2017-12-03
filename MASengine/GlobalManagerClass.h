////////////////////////////////////////////////////////////////////////////////
// Filename: GlobalManagerClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GLOBALMANAGERCLASS_H_
#define _GLOBALMANAGERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include "LogManagerClass.h"
#include "MemoryManagerClass.h"
#include "ModManagerClass.h"
#include "CommandManagerClass.h"
#include "ResourceManagerClass.h"
#include "SystemStateManagerClass.h"
#include "SettingsClass.h"
#include "PathManagerClass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: GlobalManagerClass
////////////////////////////////////////////////////////////////////////////////

/*!
\defgroup GlobalManager Глобальный менеджер 
@{
*/
/*!
Глобальный класс, который предоставляет основные функции, вставляется почти везде, синглтон
*/
class GlobalManagerClass
{
public:
	//global enums
	//! Перечисление типов регионов
	enum regionType {BASE, BLOCK, NATION};
public:
	
	//! Инициализация класса \param[in] filepath - инициализационный файл  \return false, если были ошибки
	bool Initialize(const std::string& filepath);
	void Shutdown();
	//! \brief Получить экземпляр класса
	static GlobalManagerClass& getI();

	//! Получение типа региона по его имени \param[in] key - название команды \return тип региона из перечисления
	regionType getRegionTypeEnum(const std::string& key);
	//! См. getRegionTypeEnum, но по хэшу имени
	regionType getRegionTypeEnum(int hash);
private:
	GlobalManagerClass();
	GlobalManagerClass(const GlobalManagerClass&);
	~GlobalManagerClass();
private:
	std::unordered_map<int, regionType> m_regionTypeMap; //!<map для перевода строковых значений в тип

	static GlobalManagerClass* m_instance; //!<Единственный экземпляр класса
};
/*! @} */
#endif
