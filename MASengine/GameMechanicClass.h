////////////////////////////////////////////////////////////////////////////////
// Filename: GameMechanicClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GAMEMECHANICCLASS_H_
#define _GAMEMECHANICCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "GlobalManagerClass.h"
#include "ProvRegionManagerClass.h"
#include "LoadScreenManagerClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GameMechanicClass
{
public:
	GameMechanicClass();
	GameMechanicClass(const GameMechanicClass&);
	~GameMechanicClass();

	bool Initialize();
	void Shutdown();

	//getters
	int getProvRegionID(GlobalManagerClass::regionType region, int provID);

	//setters
	void setProvRegion(GlobalManagerClass::regionType region, int provID, int regionID);
};

#endif