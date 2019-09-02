#define _SILENCE_TR2_SYS_NAMESPACE_DEPRECATION_WARNING

////////////////////////////////////////////////////////////////////////////////
// Filename: ModManagerClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _MODMANAGERCLASS_H_
#define _MODMANAGERCLASS_H_

//////////////
// INCLUDES //
//////////////


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include <fstream>
#include <map>
#include <set>
#include <iostream>
#include <filesystem>
#include <string>
#include "MemoryManagerClass.h"


using namespace std::tr2::sys;

////////////////////////////////////////////////////////////////////////////////
// Class name: ModManagerClass
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup GlobalManager 
@{
*/
/*!
Класс для загрузки модов, синглтон
*/
class ModManagerClass
{
public:

	//! \brief Инициализировать менеджер модов \param[in] filepath - путь расположения модов \return false, если были ошибки
	bool Initialize(const std::string& modDirectory);
	void Shutdown();
	
	//! \brief Получить экземпляр класса
	static ModManagerClass& getI();

	//! \brief Проверить, входит ли файл в загруженные моды \param[in] filename - полное имя файла  
	//! /return директорию файла с учетом модов или "" если файл не модифицировался
	std::string getDirectory(const std::string& filename);
	//! \brief см. getDirectory, но по хэшу от имени файла \param[in] hash - хэш от полного имени файла
	std::string getDirectory(int hash);
	

private:
	ModManagerClass();
	ModManagerClass(const ModManagerClass&);
	~ModManagerClass();
	
	/*!
	\brief загрузить мод
	\param[in] name - название мода
	\param[in] modDirectory - путь расположения мода
	\details Мод должен распологаться в папке с названием, аналогичным его имени. В папке иеархия изменяемых файлов должна соответствовать исходной.
	*/
	void loadMod(const std::string& name, const std::string& modDirectory);
private:
	static ModManagerClass* m_instance; //!<Единственный экземпляр класса

	std::map<long long, std::string> m_modChanges;  //!<Словарь с хэшами имен файлов и модифицированных директорий к ним
	std::vector<std::string> m_modNames; //!<Имена загруженных модов

	
};
/*! @} */
#endif
