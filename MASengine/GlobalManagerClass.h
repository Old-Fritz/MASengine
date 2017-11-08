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
class GlobalManagerClass
{
public:
	//global enums
	enum regionType {BASE, BLOCK, NATION};
public:
	

	bool Initialize(const std::string& filepath);
	void Shutdown();

	regionType getRegionTypeEnum(int hash);
	regionType getRegionTypeEnum(const std::string& key);

	static GlobalManagerClass& getI();

private:
	GlobalManagerClass();
	GlobalManagerClass(const GlobalManagerClass&);
	~GlobalManagerClass();
private:
	std::unordered_map<int, regionType> m_regionTypeMap;

	static GlobalManagerClass* m_instance;
};

#endif