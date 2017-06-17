
////////////////////////////////////////////////////////////////////////////////
// Filename: ModManagerClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _MODMANAGERCLASS_H_
#define _MODMANAGERCLASS_H_

//////////////
// INCLUDES //
//////////////


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include <string>
#include <fstream>
#include <map>
#include <iostream>
#include <filesystem>
#include "MemoryManagerClass.h"

using namespace std::tr2::sys;

////////////////////////////////////////////////////////////////////////////////
// Class name: ModManagerClass
////////////////////////////////////////////////////////////////////////////////

class ModManagerClass
{
public:

	ModManagerClass();
	ModManagerClass(const ModManagerClass&);
	~ModManagerClass();

	bool Initialize(const std::string& modDirectory);
	void Shutdown();


	long long getHash(const std::string& str);
	std::string getDirectory(long long hash);

private:
	void loadMod(const std::string& name, const std::string& modDirectory);
private:
	std::map<long long, std::string> m_modChanges;
	std::vector<std::string> m_modNames;
};

/////////////
// GLOBALS //
/////////////
static ModManagerClass ModManager;

#endif
