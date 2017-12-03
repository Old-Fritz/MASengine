////////////////////////////////////////////////////////////////////////////////
// Filename: provclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _PROVCLASS_H_
#define _PROVCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include"GlobalManagerClass.h"
#include "LayersClass.h"
#include <set>

////////////////////////////////////////////////////////////////////////////////
// Class name: ProvClass
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup Provs
@{
* /
/*!
Одна провинция в игре
*/
class ProvClass
{

public:
	ProvClass();
	ProvClass(const ProvClass&);
	~ProvClass();

	//! Инициализация провинции \param[in] file - файл, откуда берется информация о провинции \param[in] id - ID текущей провинции \return false, если были ошибки
	bool Initialize(std::ifstream* file, int id);
	void Shutdown();

	//void save(string filename);

	//Getters
	//! Получение ID провинции \return ID провинции
	int getID();
	//! Получение имени провинции \return Имя провинции
	std::string getName();
	//! Получение координат провинции \return Координаты провинции
	D3DXVECTOR3 getCoords();
	//! Получение слоев цвета провинции \return Слои цвета провинции
	LayersClass* getLayers();

	//Prov Regions
	//! Установка принадлежности к новому региону \param[in] type - тип региона, к которому добавляется эта прова
	//! \param[in] regionID - ID региона, к которому  добавляется эта прова
	void addRegion(GlobalManagerClass::regionType type, int regionID);
	//! Убирание принадлежности к новому региону \param[in] type - тип региона, из которого удалется эта прова
	//! \param[in] regionID - ID региона, из которого удалется эта прова
	void deleteRegion(GlobalManagerClass::regionType type, int regionID);
	//! Получение ID всех регионов определенного типа, к которому принадлежит провинция \param[0] type - тип региона 
	//! \return Вектор всех ID регионов, к которому принадлежит прова
	std::vector<int> getRegions(GlobalManagerClass::regionType type);
private:
	//void readFromFile(string filename, FractionManagerClass* fractions);
	//! Чтение из файла \param[in] file - файл, откуда берется информация \return false, если были ошибки
	bool readFromFile(std::ifstream* file);

	//! Удаление определенного элемента из вектора целых чисел \param[in, out] vec - исходный вектор целых чисел \param[in] - значение, которое надо удалить
	void erase(std::vector<int> &vec, int value);
private:
	int m_id; //!<ID провинции
	std::string m_name; //!<Имя провинции

	LayersClass* m_layers; //!<Слои цветов

	D3DXVECTOR3 m_coords; //!<Координаты провинции

	//Prov Regions
	std::vector<int> m_baseRegion;  //!<Вектор ID базовых регионов, к которым принадлежит прова
	std::vector<int> m_blockRegion; //!<Вектор ID блоковых регионов, к которым принадлежит прова
	std::vector<int> m_nationRegion; //!<Вектор ID регионов страны, к которым принадлежит прова
};
/*! @} */
#endif