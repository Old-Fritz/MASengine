////////////////////////////////////////////////////////////////////////////////
// Filename: ListElementclass.cpp
////////////////////////////////////////////////////////////////////////////////

#include "listElementclass.h"

ListElementClass::ListElementClass()
{
	InterfaceElementClass();
	m_sliderFilename = 0;
	m_elementFilename = 0;
}
ListElementClass::ListElementClass(const ListElementClass&)
{

}
ListElementClass::~ListElementClass()
{

}

bool ListElementClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, PathClass* filename,
	int screenWidth, int screenHeight)
{
	bool result;

	m_sliderFilename = PathManagerClass::getI().makePath();
	m_elementFilename = PathManagerClass::getI().makePath();

	result = (this)->InterfaceElementClass::Initialize(device, deviceContext, hwnd, filename, screenWidth, screenHeight);
	if (!result)
		return false;

	//Initialize of extra elements

	m_slider = new(1) SliderElementClass;
	result = m_slider->Initialize(device, deviceContext, hwnd, m_sliderFilename, screenWidth, screenHeight);
	if (!result)
		return false;

	return true;
}

void ListElementClass::Shutdown()
{
	clear();

	//delete slider
	if (m_slider)
	{
		m_slider->Shutdown();
		::operator delete(m_slider, sizeof(*m_slider), 1);
		m_slider = 0;
	}

	this->InterfaceElementClass::Shutdown();
}

bool ListElementClass::Render(FontShaderClass* fontShader, InterfaceShaderClass* interfaceShader, ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
	D3DXMATRIX orthoMatrix, D3DXMATRIX baseViewMatrix)
{
	bool result;
	result = this->InterfaceElementClass::Render(fontShader, interfaceShader, deviceContext, worldMatrix, orthoMatrix, baseViewMatrix);
	if (!result)
		return false;

	for (int i = 0; i < m_length; i++)
		if (m_elements[i]->isVisible())
		{
			result = m_elements[i]->Render(fontShader, interfaceShader, deviceContext, worldMatrix, orthoMatrix, baseViewMatrix);
			if (!result)
				return false;
		}
	if (m_slider->isVisible())
	{
		result = m_slider->Render(fontShader, interfaceShader, deviceContext, worldMatrix, orthoMatrix, baseViewMatrix);
		if (!result)
			return false;
	}

	return true;
}

void ListElementClass::setVisible(bool visible)
{
	this->InterfaceElementClass::setVisible(visible);

	CommandManagerClass::getI().addChange("NUM" + m_name, m_length - 1);
	CommandManagerClass::getI().addChange("ELSLENGTH" + m_name, m_length);
	CommandManagerClass::getI().addChange("MAXSIZE" + m_name, m_maxSize);
	CommandManagerClass::getI().addChange("LASTEL" + m_name, m_lastElement);
}

bool ListElementClass::addElement(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd)
{
	bool result;

	InterfaceElementClass* element;
	if (m_elType == "slider")
		element = new(4) SliderElementClass;
	else if (m_elType == "list")
		element = new(4) ListElementClass;
	else
		element = new(4) InterfaceElementClass;


	if (!element)
		return false;

	result = element->Initialize(device, deviceContext, hwnd, m_elementFilename, m_screenWidth, m_screenHeight);
	if (!result)
	{
		return false;
	}

	// place element to his place
	if (m_orientation)
		element->setPos(deviceContext, m_posX + m_elementPosX + m_length * m_interval, m_posY + m_elementPosY);
	else
		element->setPos(deviceContext, m_posX + m_elementPosX, m_posY + m_elementPosY + m_length * m_interval);


	m_elements.emplace_back(element);
	m_length++;

	// if 1 element is invisble we need slider
	if (m_length - 1 == m_maxSize)
		m_slider->setVisible(1);

	// if all elements are visible last element is up
	if (m_length <= m_maxSize)
	{
		m_lastElement++;
		m_elements[m_lastElement]->setVisible(1); // set visible is in field of view
		CommandManagerClass::getI().addChange("NUM" + m_name, m_length - 1);
		CommandManagerClass::getI().addChange("ELSLENGTH" + m_name, m_length);
		CommandManagerClass::getI().addChange("MAXSIZE" + m_name, m_maxSize);
		CommandManagerClass::getI().addChange("LASTEL" + m_name, m_lastElement);

	}

	//set num of slider`s sectors to num of invisble elements + 1 (group of visible elements)
	m_slider->updateSectors(m_length - m_maxSize + 1);
	m_slider->updateSliderPos();

}
void ListElementClass::deleteElement(ID3D11DeviceContext* deviceContext, int ind)
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

		// hide slider if all elements are visible
		if (m_length == m_maxSize)
			m_slider->setVisible(0);


		if (m_length == m_lastElement)
			m_lastElement--;

		//update all elements
		setLastElement(deviceContext, m_lastElement);
	}
}
void ListElementClass::setLastElement(ID3D11DeviceContext* deviceContext, int ind)
{
	m_lastElement = ind;

	//make all elements invisble for start
	for (int i = 0; i < m_length; i++)
	{
		m_elements[i]->setVisible(0);
	}
	//normalize last element
	if (m_lastElement >= m_length)
		m_lastElement = m_length - 1;

	if (m_lastElement < m_maxSize)
	{
		for (int i = 0; i < m_maxSize && i < m_length; i++)
		{
			// place element to his place
			if (m_orientation)
				m_elements[i]->setPos(deviceContext,m_posX + m_elementPosX + i * m_interval, m_posY + m_elementPosY);
			else
				m_elements[i]->setPos(deviceContext,m_posX + m_elementPosX, m_posY + m_elementPosY + i * m_interval);
			m_elements[i]->setVisible(1);
			m_lastElement = i;
		}
	}
	else
	{
		for (int i = 0; i < m_maxSize; i++)
		{
			// place element to his place
			if (m_orientation)
				m_elements[i + m_lastElement - m_maxSize + 1]->setPos(deviceContext, m_posX + m_elementPosX + i * m_interval, m_posY + m_elementPosY);
			else
				m_elements[i + m_lastElement - m_maxSize + 1]->setPos(deviceContext, m_posX + m_elementPosX, m_posY + m_elementPosY + i * m_interval);
			m_elements[i + m_lastElement - m_maxSize + 1]->setVisible(1);
		}
	}
	m_slider->setCurrentSector(m_lastElement - m_maxSize + 1);
	m_slider->updateSliderPos();
}
void ListElementClass::clear()
{
	m_elements.clear();
	m_length = 0;
	m_slider->setVisible(0);
	m_lastElement = -1;
}
void ListElementClass::updateDrag(ID3D11DeviceContext* deviceContext, int posX, int posY)
{
	//update drag for all elements
	this->InterfaceElementClass::updateDrag(deviceContext,posX, posY);
	m_slider->updateDrag(deviceContext,posX, posY);
	for (int i = 0; i < m_length; i++)
	{
		m_elements[i]->updateDrag(deviceContext, posX, posY);
	}
}

void ListElementClass::startDrag(int posX, int posY)
{
	this->InterfaceElementClass::startDrag(posX, posY);
	m_slider->startDrag(posX, posY);
	for (int i = 0; i < m_length; i++)
		m_elements[i]->startDrag(posX, posY);
}

void ListElementClass::endDrag()
{
	this->InterfaceElementClass::endDrag();
	m_slider->endDrag();
	for (int i = 0; i < m_length; i++)
		m_elements[i]->endDrag();
}



//Actions on click
std::string ListElementClass::pick(int posX, int posY)
{
	std::string name;
	//check fo slider pick
	name = m_slider->pick(posX, posY);
	if (name != "")
		return "_0_" + name;

	//check for elements pick
	for (int i = 0; i < m_elements.size(); i++)
	{
		name = m_elements[i]->pick(posX, posY);
		if (name != "")
			return "_" + std::to_string(i + 1) + "_" + name;
	}

	//check for main pick
	return (this)->InterfaceElementClass::pick(posX, posY);
}
void ListElementClass::addSelCommand(const std::string& name)
{
	//if getting sub element 
	if (name.size() > 0 && name[0] == '_')
	{
		std::string subName = name;
		int num = getNumFromName(name);
		if (num == 0)
		{
			//erase first part with num
			subName.erase(0, 1);
			subName.erase(0, subName.find_first_of('_') + 1);
			m_slider->addSelCommand(subName);
		}
		else if (num > 0 && num <= m_elements.size())
		{
			//erase first part with num
			subName.erase(0, 1);
			subName.erase(0, subName.find_first_of('_') + 1);
			m_elements[num - 1]->addSelCommand(subName);
			CommandManagerClass::getI().addChange("NUM" + m_name, m_length - 1);
		}
	}

	//else get from main element
	else
	{
		this->InterfaceElementClass::addSelCommand(name);
	}
	CommandManagerClass::getI().addChange("ELSLENGTH" + m_name, m_length);
	CommandManagerClass::getI().addChange("MAXSIZE" + m_name, m_maxSize);
	CommandManagerClass::getI().addChange("LASTEL" + m_name, m_lastElement);
}
void ListElementClass::addUnSelCommand(const std::string& name)
{
	//if getting sub element 
	if (name.size() > 0 && name[0] == '_')
	{
		std::string subName = name;
		int num = getNumFromName(name);
		if (num == 0)
		{
			//erase first part with num
			subName.erase(0, 1);
			subName.erase(0, subName.find_first_of('_') + 1);
			m_slider->addUnSelCommand(subName);
		}
		else if (num > 0 && num <= m_elements.size())
		{
			//erase first part with num
			subName.erase(0, 1);
			subName.erase(0, subName.find_first_of('_') + 1);
			m_elements[num - 1]->addUnSelCommand(subName);
			CommandManagerClass::getI().addChange("NUM" + m_name, m_length - 1);
		}
	}

	//else get from main element
	else
	{
		this->InterfaceElementClass::addUnSelCommand(name);
	}
	CommandManagerClass::getI().addChange("ELSLENGTH" + m_name, m_length);
	CommandManagerClass::getI().addChange("MAXSIZE" + m_name, m_maxSize);
	CommandManagerClass::getI().addChange("LASTEL" + m_name, m_lastElement);
}
void ListElementClass::addPickCommand(const std::string& name)
{
	//if getting sub element 
	if (name.size() > 0 && name[0] == '_')
	{
		std::string subName = name;
		int num = getNumFromName(name);
		if (num == 0)
		{
			//erase first part with num
			subName.erase(0, 1);
			subName.erase(0, subName.find_first_of('_') + 1);
			m_slider->addPickCommand(subName);
		}
		else if (num > 0 && num <= m_elements.size())
		{
			//erase first part with num
			subName.erase(0, 1);
			subName.erase(0, subName.find_first_of('_') + 1);
			m_elements[num - 1]->addPickCommand(subName);
			CommandManagerClass::getI().addChange("NUM" + m_name, m_length - 1);
		}
	}

	//else get from main element
	else
	{
		this->InterfaceElementClass::addPickCommand(name);
	}
	CommandManagerClass::getI().addChange("ELSLENGTH" + m_name, m_length);
	CommandManagerClass::getI().addChange("MAXSIZE" + m_name, m_maxSize);
	CommandManagerClass::getI().addChange("LASTEL" + m_name, m_lastElement);
}
void ListElementClass::addUnPickCommand(const std::string& name)
{
	//if getting sub element 
	if (name.size() > 0 && name[0] == '_')
	{
		std::string subName = name;
		int num = getNumFromName(name);
		if (num == 0)
		{
			//erase first part with num
			subName.erase(0, 1);
			subName.erase(0, subName.find_first_of('_') + 1);
			m_slider->addUnPickCommand(subName);
		}
		else if (num > 0 && num <= m_elements.size())
		{
			//erase first part with num
			subName.erase(0, 1);
			subName.erase(0, subName.find_first_of('_') + 1);
			m_elements[num - 1]->addUnPickCommand(subName);
			CommandManagerClass::getI().addChange("NUM" + m_name, m_length - 1);
		}
	}

	//else get from main element
	else
	{
		this->InterfaceElementClass::addUnPickCommand(name);
	}
	CommandManagerClass::getI().addChange("ELSLENGTH" + m_name, m_length);
	CommandManagerClass::getI().addChange("MAXSIZE" + m_name, m_maxSize);
	CommandManagerClass::getI().addChange("LASTEL" + m_name, m_lastElement);
}


//get elements
InterfaceElementClass * ListElementClass::getElement(int ind)
{
	if (ind == 0)
		return m_slider;
	if (ind <= m_length && ind > 0)
		return m_elements[ind - 1];
	else
		return NULL;
}

//work with files
bool ListElementClass::readFromFile(PathClass* filename)
{
	std::ifstream file;
	file.open(filename->getPath(), std::ios::in);
	std::string temp; // "trash"
	file >> temp >> temp >> m_name; // Getting name of element
	file >> temp >> temp >> m_bitmapsNum; // Getting number of bitmaps
	file >> temp >> temp >> m_textsNum; // Getting number of texts

	file >> temp >> temp >> m_maxSize; //  Getting number of elements
	file >> temp >> temp >> temp; //  Getting orientation of slider
	if (temp == "horizontal")
		m_orientation = 1;
	else
		m_orientation = 0;
	file >> temp >> temp >> m_interval; // Getting interval of next elements
	file >> temp >> temp >> m_elementPosX; // Getting X position of first element
	file >> temp >> temp >> m_elementPosY; // Getting Y position of first element
	file >> temp >> temp;
	file >> m_sliderFilename; // Getting filename of slider
	file >> temp >> temp;
	file >> m_elementFilename;  // Getting filename of element
	file >> temp >> temp >> m_elType; // Getting type of element

	file >> temp >> temp;
	file >> m_actionsFilename; // Getting name of file with actions

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
		GM::LM()->addLog("Error 13-7");
		return false;
	}

	if (!readTextsInfoFromFile(&file))
	{
		GM::LM()->addLog("Error 13-8");
		return false;
	}

	file.close();

	return true;
}
int ListElementClass::getNumFromName(const std::string& name)
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
