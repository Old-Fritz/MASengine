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

bool SettingsClass::Initialize(const std::string & filename)
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
void SettingsClass::save(const std::string & filename)
{
	std::ofstream file;
	file.open(filename);

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
	save(m_filename);
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
}

SettingsClass & SettingsClass::getI()
{
	if (!m_instance)
		m_instance = new(1) SettingsClass;
	return *m_instance;
}


int SettingsClass::getIntParameter(const std::string & name)
{
	if (m_intParameters.find(ModManagerClass::getI().getHash(name)) != m_intParameters.end())
		return m_intParameters.find(ModManagerClass::getI().getHash(name))->second->value;
	else
		return 0;
}
float SettingsClass::getFloatParameter(const std::string & name)
{
	if (m_floatParameters.find(ModManagerClass::getI().getHash(name)) != m_floatParameters.end())
		return m_floatParameters.find(ModManagerClass::getI().getHash(name))->second->value;
	else
		return 0;
}
std::string SettingsClass::getStrParameter(const std::string & name)
{
	if (m_strParameters.find(ModManagerClass::getI().getHash(name)) != m_strParameters.end())
		return m_strParameters.find(ModManagerClass::getI().getHash(name))->second->value;
	else
		return "";
}

void SettingsClass::setIntParameter(const std::string & name, int value)
{
	if (m_intParameters.find(ModManagerClass::getI().getHash(name)) != m_intParameters.end())
		m_intParameters.find(ModManagerClass::getI().getHash(name))->second->value = value;
}
void SettingsClass::setFloatParameter(const std::string & name, float value)
{
	if (m_floatParameters.find(ModManagerClass::getI().getHash(name)) != m_floatParameters.end())
		m_floatParameters.find(ModManagerClass::getI().getHash(name))->second->value = value;
}
void SettingsClass::setStrParameter(const std::string & name, const std::string & value)
{
	if (m_strParameters.find(ModManagerClass::getI().getHash(name)) != m_strParameters.end())
		m_strParameters.find(ModManagerClass::getI().getHash(name))->second->value = value;
}


bool SettingsClass::readFromFile(const std::string & filename)
{
	bool result;

	std::string defSettingsFileName;
	//open general file with default settings
	std::ifstream generalFile;
	generalFile.open(filename);
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
			temp = getTextFromFile(param->name, m_filename);
			if (temp.size()<=0)
				//if no settings in normal file? take default
				temp = getTextFromFile(param->name, defSettingsFileName);
			if (temp.size()>0)
				param->value = stoi(temp);
			else
				param->value = 0;
			//add parameter to map
			m_intParameters.emplace(std::pair<long long, IntParameter*>(ModManagerClass::getI().getHash(param->name),param));
			break;
		}
		//1 is FLOAT
		case(1):
		{
			FloatParameter* param = new(4) FloatParameter;
			generalFile >> param->name;
			//first check for normal settings
			temp = getTextFromFile(param->name, m_filename);
			if (temp.size() <= 0)
				//if no settings in normal file? take default
				temp = getTextFromFile(param->name, defSettingsFileName);
			if (temp.size()>0)
				param->value = stof(temp);
			else
				param->value = 0;
			//add parameter to map
			m_floatParameters.emplace(std::pair<long long, FloatParameter*>(ModManagerClass::getI().getHash(param->name), param));
			break;
		}
		//2 is STRING
		case(2):
		{
			StrParameter* param = new(4) StrParameter;
			generalFile >> param->name;
			//first check for normal settings
			temp = getTextFromFile(param->name, m_filename);
			if (temp.size() <= 0)
				//if no settings in normal file? take default
				temp = getTextFromFile(param->name, defSettingsFileName);
			param->value = temp;
			//add parameter to map
			m_strParameters.emplace(std::pair<long long, StrParameter*>(ModManagerClass::getI().getHash(param->name), param));
			break;
		}
		default:
			break;
		}
		//add new param to map
	}

	generalFile.close();

}
std::string SettingsClass::getTextFromFile(const std::string & name, const std::string & filename)
{
	std::ifstream file;
	file.open(filename);
	std::string temp1 = "}";
	std::string temp2;
	while (file)
	{
		file >> temp2;
		if (temp2 == name)
		{
			if (temp1[temp1.size() - 1] == '}')
			{
				break;
			}
		}
		temp1 = temp2;
	}
	if (temp2 != name)
	{
		file.close();
		return "";
	}
		
	else
	{
		temp1 = "";
		unsigned char a = ' ';
		while (a != '{' && file)
		{
			a = file.get();
		}
		a = file.get();
		while (a != '}' && file)
		{
			if (a < ' ')
				a = ' ';
			temp1 += a;
			a = file.get();
		}
		file.close();
		return temp1;
	}
}
