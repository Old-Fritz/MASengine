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
#include "PathClass.h"
#include "PathParamClass.h"
#include "StrParamClass.h"
#include "FloatParamClass.h"
#include "IntParamClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: SettingsClass
////////////////////////////////////////////////////////////////////////////////
class SettingsClass
{
public:

	bool Initialize(PathClass* filename);
	void save();
	void Shutdown();

	static SettingsClass& getI();

	int getIntParameter(const std::string& name);
	float getFloatParameter(const std::string& name);
	std::string getStrParameter(const std::string& name);
	PathClass* getPathParameter(const std::string& name);

	void setIntParameter(const std::string& name, int value);
	void setFloatParameter(const std::string& name, float value);
	void setStrParameter(const std::string& name, const std::string& value);
	void setPathParameter(const std::string& name, PathClass* value);

private:
	SettingsClass();
	SettingsClass(const SettingsClass&);
	~SettingsClass();
	bool readFromFile(PathClass* filename);
private:
	PathClass* m_filename;
	std::map<int, SetParamClass*> m_parameters;


	static SettingsClass* m_instance;
};

#endif