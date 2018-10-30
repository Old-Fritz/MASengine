////////////////////////////////////////////////////////////////////////////////
// Filename: GameMechanicClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GAMEMECHANICCLASS_H_
#define _GAMEMECHANICCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "GM.h"
#include "ProvRegionManagerClass.h"
#include "LoadScreenManagerClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////

/*!
\defgroup GameMechanics Игровые Механики
@{
*/
/*!
Класс для управления всеми игровыми механиками
*/
class GameMechanicClass
{
public:
	GameMechanicClass();
	GameMechanicClass(const GameMechanicClass&);
	~GameMechanicClass();

	//! Инициализация класса  \return false, если были ошибки
	bool Initialize();
	void Shutdown();

	//getters 
	//! Получение ID региона из провы \param[in] region - тип региона \param[in] provID - ID провы \return ID типа региона, указанного типа
	int getProvRegionID(GM::regionType region, int provID);

	//setters
	//! Добавление новой провинции в регион \param[in] region - тип региона \param[in] provID - ID провы \param[in] regionID - ID региона, в который будет добавлена прова
	void setProvRegion(GM::regionType region, int provID, int regionID);
};
/*! @} */
#endif