////////////////////////////////////////////////////////////////////////////////
// Filename: SettingsClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SETTINGSCLASS_H_
#define _SETTINGSCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <filesystem>
#include<string>
#include<vector>
#include<fstream>
#include <map>
#include "MemoryManagerClass.h"
#include "ModManagerClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: SettingsClass
////////////////////////////////////////////////////////////////////////////////
class SettingsClass
{

private:
	struct Parameter
	{
		std::string name;
		void* pointer;
		size_t size;
		int type;
	};
public:
	SettingsClass();
	SettingsClass(const SettingsClass&);
	~SettingsClass();

	bool Initialize(const std::string& filename);
	void save(const std::string& filename);
	void Shutdown();

	void* getParameter(const std::string& name);
	void setParameter(const std::string& name, void* pointer);

private:
	bool readFromFile(const std::string&  filename);
	const std::string& getTextFromFile(const std::string&  name, const std::string&  filename);
private:
	std::string m_filename;
	std::map<long long, Parameter> m_parameters;

};


/////////////
// GLOBALS //
/////////////
static SettingsClass Settings;

#endif