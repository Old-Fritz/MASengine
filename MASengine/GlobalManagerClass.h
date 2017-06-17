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


////////////////////////////////////////////////////////////////////////////////
// Class name: GlobalManagerClass
////////////////////////////////////////////////////////////////////////////////
class GlobalManagerClass
{
public:
	GlobalManagerClass();
	GlobalManagerClass(const GlobalManagerClass&);
	~GlobalManagerClass();

	bool Initialize(const std::string& filepath);
	void Shutdown();
};

/////////////
// GLOBALS //
/////////////
static GlobalManagerClass GlobalManager;

#endif