#include "GameMechanicClass.h"

GameMechanicClass::GameMechanicClass()
{
}

GameMechanicClass::GameMechanicClass(const GameMechanicClass &)
{
}

GameMechanicClass::~GameMechanicClass()
{
}

bool GameMechanicClass::Initialize()
{
	bool result;

	//Initialize prov manager
	if (&(ProvManagerClass::getI()))
	{
		result = ProvManagerClass::getI().Initialize(SettingsClass::getI().getPathParameter("ProvFilename"));
		if (!result)
		{
			return false;
		}
	}
	else
		return false;
	LogManagerClass::getI().addLog("Provs Initialization");
	ProvManagerClass::getI().setGlobalMainColor(GlobalManagerClass::NATION);

	//Initialize prov region manager
	if (&(ProvRegionManagerClass::getI()))
	{
		result = ProvRegionManagerClass::getI().Initialize(SettingsClass::getI().getPathParameter("BaseRegionFilename"));
		if (!result)
		{
			return false;
		}
	}
	else
		return false;
	LogManagerClass::getI().addLog("Provs Regions Initialization");

	//init test regions
	std::ifstream file;
	file.open(SettingsClass::getI().getPathParameter("ProvRegionFilename")->getPath());
	NationRegionClass* reg1 = new(4) NationRegionClass;
	reg1->Initialize(&file, 1);
	NationRegionClass* reg2 = new(4) NationRegionClass;
	reg2->Initialize(&file, 2);
	ProvRegionManagerClass::getI().addProvRegion(GlobalManagerClass::NATION, reg1);
	ProvRegionManagerClass::getI().addProvRegion(GlobalManagerClass::NATION, reg2);

	return true;
}

void GameMechanicClass::Shutdown()
{
	if (&(ProvRegionManagerClass::getI()))
	{
		ProvRegionManagerClass::getI().Shutdown();
	}

	if (&(ProvManagerClass::getI()))
	{
		ProvManagerClass::getI().Shutdown();
	}
}

int GameMechanicClass::getProvRegionID(GlobalManagerClass::regionType region, int provID)
{
	return ProvManagerClass::getI().getProv(provID)->getRegions(region).back();
}

void GameMechanicClass::setProvRegion(GlobalManagerClass::regionType region, int provID, int regionID)
{
	ProvRegionManagerClass::getI().getProvRegion(region, regionID)->add(provID);
}
