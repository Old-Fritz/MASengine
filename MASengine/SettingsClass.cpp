#include "SettingsClass.h"

SettingsClass* SettingsClass::m_instance = 0;

SettingsClass::SettingsClass()
{
	m_filename = 0;
}
SettingsClass::SettingsClass(const SettingsClass &)
{
}
SettingsClass::~SettingsClass()
{
}

bool SettingsClass::Initialize(PathClass* filename)
{
	bool result;

	m_filename = PathManagerClass::getI().makePath();
	result = readFromFile(filename);
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 5-1");
		return false;
	}

	return true;
}
void SettingsClass::save()
{
	std::ofstream file;
	file.open(m_filename->getPath());

	if (!file.is_open())
		return;

	//write all params in file
	for (auto param = m_parameters.begin();param != m_parameters.end();param++)
		file << param->second->getName() << " {" << param->second->getStr() << "}" << std::endl;

	file.close();
}
void SettingsClass::Shutdown()
{
	//delete all params
	while (!m_parameters.empty())
	{
		m_parameters.begin()->second->Shutdown();
		::operator delete(m_parameters.begin()->second,sizeof(m_parameters.begin()->second), 2);
		m_parameters.erase(m_parameters.begin());
	}

	if (m_instance)
	{
		::operator delete(m_instance, sizeof(*m_instance), 1);
		m_instance = 0;
	}
}

SettingsClass & SettingsClass::getI()
{
	if (!m_instance)
		m_instance = new(1) SettingsClass;
	return *m_instance;
}


int SettingsClass::getIntParameter(const std::string & name)
{
	int hash = Utils::getHash(name);
	if (m_parameters.find(hash) != m_parameters.end())
		return ((IntParamClass*)(m_parameters.find(hash)->second))->getParam();
	else
		return 0;
}
float SettingsClass::getFloatParameter(const std::string & name)
{
	int hash = Utils::getHash(name);
	if (m_parameters.find(hash) != m_parameters.end())
		return ((FloatParamClass*)(m_parameters.find(hash)->second))->getParam();
	else
		return 0;
}
std::string SettingsClass::getStrParameter(const std::string & name)
{
	int hash = Utils::getHash(name);
	if (m_parameters.find(hash) != m_parameters.end())
		return ((StrParamClass*)(m_parameters.find(hash)->second))->getParam();
	else
		return "";
}

PathClass * SettingsClass::getPathParameter(const std::string & name)
{
	int hash = Utils::getHash(name);
	if (m_parameters.find(hash) != m_parameters.end())
		return ((PathParamClass*)(m_parameters.find(hash)->second))->getParam();
	else
		return NULL;
}

void SettingsClass::setIntParameter(const std::string & name, int value)
{
	int hash = Utils::getHash(name);
	if (m_parameters.find(hash) != m_parameters.end())
		((IntParamClass*)(m_parameters.find(hash)->second))->setParam(value);
}
void SettingsClass::setFloatParameter(const std::string & name, float value)
{
	int hash = Utils::getHash(name);
	if (m_parameters.find(hash) != m_parameters.end())
		((FloatParamClass*)(m_parameters.find(hash)->second))->setParam(value);
}
void SettingsClass::setStrParameter(const std::string & name, const std::string & value)
{
	int hash = Utils::getHash(name);
	if (m_parameters.find(hash) != m_parameters.end())
		((StrParamClass*)(m_parameters.find(hash)->second))->setParam(value);
}

void SettingsClass::setPathParameter(const std::string & name, PathClass * value)
{
	int hash = Utils::getHash(name);
	if (m_parameters.find(hash) != m_parameters.end())
		((PathParamClass*)(m_parameters.find(hash)->second))->setParam(value);
}


bool SettingsClass::readFromFile(PathClass* filename)
{
	bool result;

	PathClass* defSettingsFileName = PathManagerClass::getI().makePath();
	//open general file with default settings
	std::ifstream generalFile;
	generalFile.open(filename->getPath());
	if (!generalFile.is_open())
	{
		LogManagerClass::getI().addLog("Error 5-2");
		return false;
	}
	
	//first value is filename
	generalFile >> m_filename;
	generalFile >> defSettingsFileName;

	//load all params
	int paramsNum;
	generalFile >> paramsNum;
	
	for (int i = 0;i < paramsNum;i++)
	{
		SetParamClass* param;
		std::string paramName;
		std::string paramText;
		//get type of param and name first
		int valueType;
		generalFile >> valueType;
		generalFile >> paramName;
		//first check for normal settings
		paramText = Utils::getTextFromFile(paramName, m_filename->getPath());
		if (paramText.size() <= 0)
			//if no settings in normal file, take default
			paramText = Utils::getTextFromFile(paramName, defSettingsFileName->getPath());
		switch (valueType)
		{
		//0 is INT
		case(0):
		{
			param = new(4) IntParamClass;
			break;
		}
		//1 is FLOAT
		case(1):
		{
			param = new(4) FloatParamClass;
			break;
		}
		//2 is STRING
		case(2):
		{
			param = new(4) StrParamClass;
			break;
		}
		//3 is PATH
		case(3):
		{
			param = new(4) PathParamClass;
			break;
		}
		default:
			param = new(4) SetParamClass;
			break;
		}
		//fill param with data
		param->setStr(paramText);
		param->setName(paramName);
		//add new param to map
		m_parameters.emplace(std::pair<int, SetParamClass*>(Utils::getHash(paramName), param));
	}

	generalFile.close();

}