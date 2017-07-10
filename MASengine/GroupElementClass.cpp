#include "GroupElementClass.h"

GroupElementClass::GroupElementClass()
{
}
GroupElementClass::GroupElementClass(const GroupElementClass &)
{
}
GroupElementClass::~GroupElementClass()
{
}

bool GroupElementClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, const std::string& filename,
	int screenWidth, int screenHeight)
{
	return this->InterfaceElementClass::Initialize(device, deviceContext, hwnd, filename, screenWidth, screenHeight);
}
void GroupElementClass::Shutdown()
{
	clear();
	this->InterfaceElementClass::Shutdown();
}
bool GroupElementClass::Render(FontShaderClass* fontShader, InterfaceShaderClass* interfaceShader, ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
	D3DXMATRIX orthoMatrix, D3DXMATRIX baseViewMatrix)
{
	this->InterfaceElementClass::Render(fontShader, interfaceShader, deviceContext, worldMatrix, orthoMatrix, baseViewMatrix);
	for (int i = 0; i < m_length; i++)
		if (m_elements[i]->isVisible())
			return m_elements[i]->Render(fontShader, interfaceShader, deviceContext, worldMatrix, orthoMatrix, baseViewMatrix);
}

void GroupElementClass::setVisible(bool visible)
{
	this->InterfaceElementClass::setVisible(visible);

	CommandManagerClass::getI().addChange("NUM" + m_name, m_length - 1);
	CommandManagerClass::getI().addChange("ELSLENGTH" + m_name, m_length);
}

//Elements changing
bool GroupElementClass::addElement(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd)
{
	bool result;
	InterfaceElementClass* element;

	//choose type
	if (m_elType == "slider")
		element = new(4) SliderElementClass;
	else if (m_elType == "list")
		element = new(4) ListElementClass;
	else if (m_elType == "group")
		element = new(4) GroupElementClass;
	else
		element = new(4) InterfaceElementClass;

	//init element and add to vector
	result = element->Initialize(device, deviceContext, hwnd, m_elementFilename, m_screenWidth, m_screenHeight);
	if (!result)
		return false;

	m_elements.emplace_back(element);
	m_length++;


	m_elements[m_length - 1]->setVisible(1);

	//make changes
	CommandManagerClass::getI().addChange("NUM" + m_name, m_length - 1);
	CommandManagerClass::getI().addChange("ELSLENGTH" + m_name, m_length);

	return true;
}
void GroupElementClass::deleteElement(int ind)
{

	if (ind >= 0 && ind < m_length)
	{
		if (m_elements[ind])
		{
			m_elements[ind]->Shutdown();
			::operator delete(m_elements[ind], sizeof(*m_elements[ind]), 2);
			m_elements[ind] = 0;
		}
		m_elements.erase(m_elements.begin() + ind);
		m_length--;
	}
}
void GroupElementClass::clear()
{
	for (int i = 0; i < m_length; i++)
	{
		deleteElement(0);
	}
	m_length = 0;
}

void GroupElementClass::updateDrag(ID3D11DeviceContext* deviceContext, int posX, int posY)
{
	//update drag for all elements
	this->InterfaceElementClass::updateDrag(deviceContext, posX, posY);
	for (int i = 0; i < m_length; i++)
	{
		m_elements[i]->updateDrag(deviceContext, posX, posY);
	}
}

void GroupElementClass::startDrag(int posX, int posY)
{
	this->InterfaceElementClass::startDrag(posX, posY);
	for (int i = 0; i < m_length; i++)
		m_elements[i]->startDrag(posX, posY);
}

void GroupElementClass::endDrag()
{
	this->InterfaceElementClass::endDrag();
	for (int i = 0; i < m_length; i++)
		m_elements[i]->endDrag();
}

//Actions on click
std::string GroupElementClass::pick(int posX, int posY)
{
	std::string name;

	//check for elements pick
	for (int i = m_elements.size() - 1; i >= 0; i--)
	{
		name = m_elements[i]->pick(posX, posY);
		if (name != "")
			return "_" + std::to_string(i) + "_" + name;
	}

	//check for main pick
	return (this)->InterfaceElementClass::pick(posX, posY);
}
void GroupElementClass::addSelCommand(const std::string& name)
{
	//if getting sub element 

	if (name.size() > 0 && name[0] == '_')
	{
		int num = getNumFromName(name);
		if (num >= 0 && num < m_elements.size())
		{
			//erase first part with num
			std::string  subName = name;
			subName.erase(0, 1);
			subName.erase(0, name.find_first_of('_') + 1);
			m_elements[num]->addSelCommand(subName);

			CommandManagerClass::getI().addChange("NUM" + m_name, num);
		}
	}

	//else get from main element
	else
	{
		this->InterfaceElementClass::addSelCommand(name);
	}

	CommandManagerClass::getI().addChange("ELSLENGTH" + m_name, m_length);

}
void GroupElementClass::addUnSelCommand(const std::string&  name)
{
	//if getting sub element 

	if (name.size() > 0 && name[0] == '_')
	{
		int num = getNumFromName(name);
		if (num >= 0 && num < m_elements.size())
		{
			//erase first part with num
			std::string  subName = name;
			subName.erase(0, 1);
			subName.erase(0, name.find_first_of('_') + 1);
			m_elements[num]->addUnSelCommand(subName);

			CommandManagerClass::getI().addChange("NUM" + m_name, num);
		}
	}

	//else get from main element
	else
	{
		this->InterfaceElementClass::addUnSelCommand(name);
	}

	CommandManagerClass::getI().addChange("ELSLENGTH" + m_name, m_length);
}
void GroupElementClass::addPickCommand(const std::string&  name)
{
	//if getting sub element 

	if (name.size() > 0 && name[0] == '_')
	{
		int num = getNumFromName(name);
		if (num >= 0 && num < m_elements.size())
		{
			//erase first part with num
			std::string  subName = name;
			subName.erase(0, 1);
			subName.erase(0, name.find_first_of('_') + 1);
			m_elements[num]->addPickCommand(subName);

			CommandManagerClass::getI().addChange("NUM" + m_name, num);
		}
	}

	//else get from main element
	else
	{
		this->InterfaceElementClass::addPickCommand(name);
	}

	CommandManagerClass::getI().addChange("ELSLENGTH" + m_name, m_length);
}
void GroupElementClass::addUnPickCommand(const std::string&  name)
{
	//if getting sub element 

	if (name.size() > 0 && name[0] == '_')
	{
		int num = getNumFromName(name);
		if (num >= 0 && num < m_elements.size())
		{
			//erase first part with num
			std::string  subName = name;
			subName.erase(0, 1);
			subName.erase(0, name.find_first_of('_') + 1);
			m_elements[num]->addUnPickCommand(subName);

			CommandManagerClass::getI().addChange("NUM" + m_name, num);
		}
	}

	//else get from main element
	else
	{
		this->InterfaceElementClass::addUnPickCommand(name);
	}

	CommandManagerClass::getI().addChange("ELSLENGTH" + m_name, m_length);
}

//get elements
InterfaceElementClass * GroupElementClass::getElement(int ind)
{
	if (ind >= 0 && ind < m_length)
		return m_elements[ind];
	else
		return NULL;
}

//Work with files
bool GroupElementClass::readFromFile(const std::string&  filename)
{
	std::fstream file;
	file.open(filename, std::ios::in);
	std::string temp; // "trash"
	file >> temp >> temp >> m_name; // Getting name of element
	file >> temp >> temp >> m_bitmapsNum; // Getting number of bitmaps
	file >> temp >> temp >> m_textsNum; // Getting number of texts

	file >> temp >> temp >> m_elementFilename;  // Getting filename of element
	file >> temp >> temp >> m_elType; // Getting type of element

	file >> temp >> temp >> m_actionsFileName; // Getting name of file with actions

	// get width and height
	file >> temp >> temp >> temp;
	if (temp == "screenWidth")
		m_width - m_screenWidth;
	else
		m_width = atoi(temp.c_str());

	file >> temp >> temp >> temp;
	if (temp == "screenHeight")
		m_height - m_screenHeight;
	else
		m_height = atoi(temp.c_str());

	//get pos on X and Y
	file >> temp >> temp >> temp;
	if (temp == "middle")
		m_posX = m_screenWidth / 2 - m_width / 2;
	else
		m_posX = atoi(temp.c_str());

	file >> temp >> temp >> temp;
	if (temp == "middle")
		m_posY = m_screenHeight / 2 - m_height / 2;
	else
		m_posY = atoi(temp.c_str());

	// getting orientation in vertical
	file >> temp >> temp >> temp;
	if (temp == "bottom")
		m_vertOrien = 1;
	else
		m_vertOrien = 0;

	// getting orientation in horizontal
	file >> temp >> temp >> temp;
	if (temp == "right")
		m_horOrien = 1;
	else
		m_horOrien = 0;

	//read info about texts and bitmaps
	if (!readBitmapsInfoFromFile(&file))
	{
		LogManagerClass::getI().addLog("Error 13-7");
		return false;
	}

	if (!readTextsInfoFromFile(&file))
	{
		LogManagerClass::getI().addLog("Error 13-8");
		return false;
	}

	file.close();

	return true;
}

int GroupElementClass::getNumFromName(const std::string& name)
{
	//find index of last zifra of number
	int indOfLastZ = -1;
	for (int i = 1; i < name.size(); i++)
	{
		if (name[i] == '_')
		{
			indOfLastZ = i;
			break;
		}
	}

	//check if name is correct
	if (indOfLastZ < 0)
		return -1;

	//get number from string
	std::string number;
	for (int i = 1; i < indOfLastZ; i++)
		number += name[i];
	return (new CalculatorClass)->calculate(number);
}
