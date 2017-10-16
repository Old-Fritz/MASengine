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
	enum regionType {RESERVE0, RESERVE1, RESERVE2, BASE, BLOCK, NATION};
public:
	

	bool Initialize(const std::string& filepath);
	void Shutdown();

	static GlobalManagerClass& getI();

private:
	GlobalManagerClass();
	GlobalManagerClass(const GlobalManagerClass&);
	~GlobalManagerClass();
private:
	static GlobalManagerClass* m_instance;
};

#endif