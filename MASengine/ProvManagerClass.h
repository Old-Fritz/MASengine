////////////////////////////////////////////////////////////////////////////////
// Filename: ProvManagerClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _PROVMANAGERCLASS_H_
#define _PROVMANAGERCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "provclass.h"
#include "LoadScreenManagerClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ProvClass
////////////////////////////////////////////////////////////////////////////////

/*!
\defgroup Provs Провинции
\ingroup GameMechanics
@{
* /
/*!
Класс для управления всеми провинциями, синглтон
*/
class ProvManagerClass
{

public:
	//!Инициализация класса и загрузка пров \param[in] filename - файл с данными о провинциях \return false, если были ошибки
	bool Initialize(PathClass* filename);
	void Shutdown();

	//Getters
	//! \brief Получение провинции по ID \param[in] provID - ID провинции \return провинция по указанному ID
	//! \details Если ID некорректен, то возвращается провинция с ID = 0
	ProvClass* getProv(int provID);
	//!Установить новый основной цвет для всех пров \param[in] type - тип региона, цвет которого будет выбран основным
	void setGlobalMainColor(GM::regionType type);

	//! Получение количества пров \return количество инициализированных провинций
	int getProvNum();

	//! \brief Получить экземпляр класса
	static ProvManagerClass& getI();

private:
	ProvManagerClass();
	ProvManagerClass(const ProvManagerClass&);
	~ProvManagerClass();
private:
	static ProvManagerClass* m_instance; //!<Единственный экземпляр класса

	std::vector<ProvClass*> m_provs; //!<Все провинции
};
/*! @} */
#endif