#include "SettingsClass.h"

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

	for (auto param = m_parameters.begin();param != m_parameters.end();param++)
	{
		switch (param->second.type)
		{
		case(0):
			file << param->second.name << " {" << *(int*)(param->second.pointer) << "}" << std::endl;
		case(1):
			file << param->second.name << " {" << *(float*)(param->second.pointer) << "}" << std::endl;
		case(2):
			file << param->second.name << " {" << *(std::string*)(param->second.pointer) << "}" << std::endl;
		default:
			file << param->second.name << " {" << *(int*)(param->second.pointer) << "}" << std::endl;;
		}
	}

	file.close();
}
void SettingsClass::Shutdown()
{
	save(m_filename);
	while (!m_parameters.empty())
	{
		MemoryManagerClass::getI().deletePool(m_parameters.begin()->second.pointer, m_parameters.begin()->second.size);
		m_parameters.erase(m_parameters.begin());
	}
}

void * SettingsClass::getParameter(const std::string & name)
{
	if (m_parameters.find(ModManager.getHash(name)) != m_parameters.end())
		return m_parameters.find(ModManager.getHash(name))->second.pointer;
	else
		return 0;
}
void SettingsClass::setParameter(const std::string & name, void * pointer)
{
	if (m_parameters.find(ModManager.getHash(name)) != m_parameters.end())
	{
		m_parameters.find(ModManager.getHash(name))->second.pointer = pointer;
	}
}


bool SettingsClass::readFromFile(const std::string & filename)
{
	bool result;

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

	//load all params
	int paramsNum;
	generalFile >> paramsNum;
	std::string temp;
	for (int i = 0;i < paramsNum;i++)
	{
		Parameter param;
		//get type of param and name first
		int valueType;
		generalFile >> valueType;
		generalFile >> param.name;
		
		param.type = valueType;
		switch (valueType)
		{
		//0 is INT
		case(0):
		{
			param.size = sizeof(int);
			param.pointer = MemoryManagerClass::getI().getPoolMemory(param.size);
			temp = getTextFromFile(param.name, m_filename);
			if (temp.size()>0)
				temp = getTextFromFile(param.name, filename);
			if (temp.size()>0)
				*(int*)param.pointer = stoi(temp);
			else
				*(int*)param.pointer = 0;
		}
		//1 is FLOAT
		case(1):
		{
			param.size = sizeof(float);
			param.pointer = MemoryManagerClass::getI().getPoolMemory(param.size);
			temp = getTextFromFile(param.name, m_filename);
			if (temp.size()>0)
				temp = getTextFromFile(param.name, filename);
			if (temp.size()>0)
				*(float*)param.pointer = stof(temp);
			else
				*(float*)param.pointer = 0;
		}
		//2 is STRING
		case(2):
		{
			param.size = sizeof(std::string);
			param.pointer = MemoryManagerClass::getI().getPoolMemory(param.size);
			temp = getTextFromFile(param.name, m_filename);
			if (temp.size()>0)
				temp = getTextFromFile(param.name, filename);
			*(std::string*)param.pointer = temp;
		}
		default:
			param.size = sizeof(int);
			param.pointer = MemoryManagerClass::getI().getPoolMemory(param.size);
			*(int*)param.pointer = 0;
		}
		//add new param to map
		m_parameters.emplace(std::pair<long long, Parameter>(ModManager.getHash(param.name), param));
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
