////////////////////////////////////////////////////////////////////////////////
// Filename: ProvRegionClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _PROVREGIONCLASS_H_
#define _PROVREGIONCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "GM.h"
#include <set>
#include "ProvManagerClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ProvClass
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup ProvRegions
@{
* /
/*!
Один регион в игре
*/
class ProvRegionClass
{

public:
	ProvRegionClass();
	ProvRegionClass(const ProvRegionClass&);
	~ProvRegionClass();

	//! Инициализация региона \param[in] file - файл, откуда берется информация о регионе \param[in] id - ID текущего региона \return false, если были ошибки
	virtual bool Initialize(std::ifstream* file, int id);
	virtual void Shutdown();

	//! Добавление провинции в регион \param[in] provID - ID добавляемой провинции
	virtual void add(int provID);
	//! Удаление провинции из региона \param[in] provID - ID удаляемой провинции
	virtual void erase(int provID);
	

	//Getters
	//! Получение ID региона \return ID провинции
	virtual int getID();
	//! Получение всех провинций региона \return вектор с указателями на провинции
	virtual std::vector<ProvClass*>* getProvs();
protected:
	//! Чтение из файла \param[in] file - файл, откуда берется информация \return false, если были ошибки
	bool readFromFile(std::ifstream* file);
protected:
	int m_id; //!<ID региона
	
	D3DXVECTOR4 m_color = D3DXVECTOR4(0,0,0,1); //!<Цвет региона

	std::vector<ProvClass*> m_provs; //!<Провы
};
/*! @} */
#endif