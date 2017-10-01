#include "SettingsClass.h"

SettingsClass* SettingsClass::m_instance = 0;

SettingsClass::SettingsClass()
{
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
	for (auto param = m_intParameters.begin();param != m_intParameters.end();param++)
		file << param->second->name << " {" << param->second->value << "}" << std::endl;
	file << std::endl;
	for (auto param = m_floatParameters.begin();param != m_floatParameters.end();param++)
		file << param->second->name << " {" << param->second->value << "}" << std::endl;
	file << std::endl;
	for (auto param = m_strParameters.begin();param != m_strParameters.end();param++)
		file << param->second->name << " {" << param->second->value << "}" << std::endl;

	file.close();
}
void SettingsClass::Shutdown()
{
	//delete all params
	while (!m_intParameters.empty())
	{
		m_intParameters.begin()->second->name.clear();
		::operator delete(m_intParameters.begin()->second,sizeof(IntParameter), 2);
		m_intParameters.erase(m_intParameters.begin());
	}
	while (!m_floatParameters.empty())
	{
		m_floatParameters.begin()->second->name.clear();
		::operator delete(m_floatParameters.begin()->second, sizeof(FloatParameter), 2);
		m_floatParameters.erase(m_floatParameters.begin());
	}
	while (!m_strParameters.empty())
	{
		m_strParameters.begin()->second->name.clear();
		m_strParameters.begin()->second->value.clear();
		::operator delete(m_strParameters.begin()->second, sizeof(StrParameter), 2);
		m_strParameters.erase(m_strParameters.begin());
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
	if (m_intParameters.find(Utils::getHash(name)) != m_intParameters.end())
		return m_intParameters.find(Utils::getHash(name))->second->value;
	else
		return 0;
}
float SettingsClass::getFloatParameter(const std::string & name)
{
	if (m_floatParameters.find(Utils::getHash(name)) != m_floatParameters.end())
		return m_floatParameters.find(Utils::getHash(name))->second->value;
	else
		return 0;
}
std::string SettingsClass::getStrParameter(const std::string & name)
{
	if (m_strParameters.find(Utils::getHash(name)) != m_strParameters.end())
		return m_strParameters.find(Utils::getHash(name))->second->value;
	else
		return "";
}

void SettingsClass::setIntParameter(const std::string & name, int value)
{
	if (m_intParameters.find(Utils::getHash(name)) != m_intParameters.end())
		m_intParameters.find(Utils::getHash(name))->second->value = value;
}
void SettingsClass::setFloatParameter(const std::string & name, float value)
{
	if (m_floatParameters.find(Utils::getHash(name)) != m_floatParameters.end())
		m_floatParameters.find(Utils::getHash(name))->second->value = value;
}
void SettingsClass::setStrParameter(const std::string & name, const std::string & value)
{
	if (m_strParameters.find(Utils::getHash(name)) != m_strParameters.end())
		m_strParameters.find(Utils::getHash(name))->second->value = value;
}


bool SettingsClass::readFromFile(PathClass* filename)
{
	bool result;

	PathClass* defSettingsFileName = new(4) PathClass;
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
	std::string temp;
	for (int i = 0;i < paramsNum;i++)
	{
		//get type of param and name first
		int valueType;
		generalFile >> valueType;		
		switch (valueType)
		{
		//0 is INT
		case(0):
		{
			IntParameter* param = new(4) IntParameter;
			generalFile >> param->name;
			//first check for normal settings
			temp = Utils::getTextFromFile(param->name, m_filename->getPath());
			if (temp.size()<=0)
				//if no settings in normal file? take default
				temp = Utils::getTextFromFile(param->name, defSettingsFileName->getPath());
			if (temp.size()>0)
				param->value = stoi(temp);
			else
				param->value = 0;
			//add parameter to map
			m_intParameters.emplace(std::pair<int, IntParameter*>(Utils::getHash(param->name),param));
			break;
		}
		//1 is FLOAT
		case(1):
		{
			FloatParameter* param = new(4) FloatParameter;
			generalFile >> param->name;
			//first check for normal settings
			temp = Utils::getTextFromFile(param->name, m_filename->getPath());
			if (temp.size() <= 0)
				//if no settings in normal file? take default
				temp = Utils::getTextFromFile(param->name, defSettingsFileName->getPath());
			if (temp.size()>0)
				param->value = stof(temp);
			else
				param->value = 0;
			//add parameter to map
			m_floatParameters.emplace(std::pair<int, FloatParameter*>(Utils::getHash(param->name), param));
			break;
		}
		//2 is STRING
		case(2):
		{
			StrParameter* param = new(4) StrParameter;
			generalFile >> param->name;
			//first check for normal settings
			temp = Utils::getTextFromFile(param->name, m_filename->getPath());
			if (temp.size() <= 0)
				//if no settings in normal file? take default
				temp = Utils::getTextFromFile(param->name, defSettingsFileName->getPath());
			param->value = temp;
			//add parameter to map
			m_strParameters.emplace(std::pair<int, StrParameter*>(Utils::getHash(param->name), param));
			break;
		}
		default:
			break;
		}
		//add new param to map
	}

	generalFile.close();

}