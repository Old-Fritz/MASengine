////////////////////////////////////////////////////////////////////////////////
// Filename: ProvRegionManagerClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _PROVREGIONMANAGERCLASS_H_
#define _PROVREGIONMANAGERCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "provRegionClass.h"
#include "NationRegionClass.h"
#include "BlockRegionClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ProvRegionManagerClass
////////////////////////////////////////////////////////////////////////////////

/*!
\defgroup ProvsRegions Регионы
\ingroup GameMechanics
@{
* /
/*!
Класс для управления всеми провинциями, синглтон
*/
class ProvRegionManagerClass
{

public:
	//!Инициализация класса и загрузка регионов \param[in] filename - файл с данными о базовых регионах \return false, если были ошибки
	bool Initialize(PathClass* filename);

	void Shutdown();

	//Getters
	//! \brief Получение провинции по ID \param[in] type - тип региона \param[in] provRegionID - ID региона \return регион по указанному ID
	//! \details Если ID некорректен, то возвращается регион с ID = 0
	ProvRegionClass* getProvRegion(GM::regionType type, int provRegionID);
	//! Добавление нового региона \param[in] type - тип региона \param[in] region - ссылка на созданный регион
	void addProvRegion(GM::regionType type, ProvRegionClass* region);

	//! \brief Получить экземпляр класса
	static ProvRegionManagerClass& getI();
private:
	ProvRegionManagerClass();
	ProvRegionManagerClass(const ProvRegionManagerClass&);
	~ProvRegionManagerClass();
private:
	static ProvRegionManagerClass* m_instance; //!<Единственный экземпляр класса

	std::vector<ProvRegionClass*> m_provRegions; //!<Базовые регионы
	std::vector<BlockRegionClass*> m_blockRegion;  //!<Блоковые регионы
	std::vector<NationRegionClass*> m_nationRegion; //!<Регионы пров
};
/*! @} */
#endif