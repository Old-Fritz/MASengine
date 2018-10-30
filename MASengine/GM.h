////////////////////////////////////////////////////////////////////////////////
// Filename: GM.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GM_H_
#define _GM_H_


//////////////
// INCLUDES //
//////////////
#include "LogManager.h"
#include "MemoryManager.h"
#include "ModManagerClass.h"
#include "CommandManagerClass.h"
#include "ResourceManagerClass.h"
#include "SystemStateManagerClass.h"
#include "SettingsClass.h"
#include "PathManagerClass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: GM
////////////////////////////////////////////////////////////////////////////////

/*!
\defgroup GlobalManager Глобальный менеджер 
@{
*/
/*!
Глобальный класс, который предоставляет основные функции, вставляется почти везде, синглтон
*/
class GM
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
	static GM& getI();

	static LogManager* LM();

	//! Получение типа региона по его имени \param[in] key - название команды \return тип региона из перечисления
	regionType getRegionTypeEnum(const std::string& key);
	//! См. getRegionTypeEnum, но по хэшу имени
	regionType getRegionTypeEnum(int hash);
private:
	GM();
	GM(const GM&);
	~GM();
private:
	std::unordered_map<int, regionType> m_regionTypeMap; //!<map для перевода строковых значений в тип

	LogManager* m_logManager;

	static GM* m_instance; //!<Единственный экземпляр класса
};
/*! @} */
#endif
