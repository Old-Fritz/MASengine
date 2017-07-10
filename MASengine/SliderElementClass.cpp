////////////////////////////////////////////////////////////////////////////////
// Filename: SliderElementclass.cpp
////////////////////////////////////////////////////////////////////////////////

#include "SliderElementclass.h"



SliderElementClass::SliderElementClass()
{

}
SliderElementClass::SliderElementClass(const SliderElementClass&)
{

}
SliderElementClass::~SliderElementClass()
{

}

bool SliderElementClass::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, HWND hwnd, const std::string & filename, int screenWidth, int screenHeight)
{
	bool result;
	result = (this)->InterfaceElementClass::Initialize(device, deviceContext, hwnd, filename, screenWidth, screenHeight);
	if (!result)
		return false;

	return true;
}

//sectors changing
void SliderElementClass::updateSectors(int num)
{
	m_sectorsNum = num;
}
int SliderElementClass::findSector(float posX, float posY)
{
	int i = findBMbyName("backLine");


	if (m_orientation) // orientation is horizontal
	{
		//normalize position
		posX -= m_bitmapsInfo[i].posX;
		if (posX <= 0)
			posX = 0.0001f;
		if (posX >= m_bitmapsInfo[i].width)
			posX = m_bitmapsInfo[i].width - 0.0001f;

		//calculate sector
		return posX / m_bitmapsInfo[i].width*m_sectorsNum;
	}
	else  // orientation is vertical
	{
		//normalize position
		posY -= m_bitmapsInfo[i].posY;
		if (posY <= 0)
			posY = 0.0001f;
		if (posY >= m_bitmapsInfo[i].height)
			posY = m_bitmapsInfo[i].height - 0.0001f;

		//calculate sector
		return posY / m_bitmapsInfo[i].height*m_sectorsNum;
	}
}
void SliderElementClass::setCurrentSector(int sector)
{
	m_currentSector = sector;
}
void SliderElementClass::updateSliderPos()
{
	int sliderInd = findBMbyName("slider");
	int backLineind = findBMbyName("backLine");

	if (m_orientation) // orientation is horizontal
	{
		m_bitmapsInfo[sliderInd].posX = m_bitmapsInfo[backLineind].posX + (m_bitmapsInfo[backLineind].width - m_bitmapsInfo[sliderInd].width) / m_sectorsNum * (m_currentSector);
	}
	else  // orientation is vertical
	{
		m_bitmapsInfo[sliderInd].posY = m_bitmapsInfo[backLineind].posY + (m_bitmapsInfo[backLineind].height - m_bitmapsInfo[sliderInd].height) / m_sectorsNum * (m_currentSector);
	}
}
void SliderElementClass::updateSector(float posX, float posY)
{
	setCurrentSector(findSector(posX, posY));
	updateSliderPos();
	addSliderCommand();
}
void SliderElementClass::setVisible(bool visible)
{
	this->InterfaceElementClass::setVisible(visible);
	CommandManagerClass::getI().addChange("NUM" + m_name, m_currentSector);

}
void SliderElementClass::updateDrag(ID3D11DeviceContext* deviceContext,int posX, int posY)
{
	if (!m_dragType)
	{
		this->InterfaceElementClass::updateDrag(deviceContext,posX, posY);
	}
	else
	{
		updateSector(posX, posY);
		m_lastX = posX;
		m_lastY = posY;
	}
}
void SliderElementClass::endDrag()
{
	this->InterfaceElementClass::endDrag();
	m_dragType = 0;
}
void SliderElementClass::nextSector()
{
	if (m_currentSector < m_sectorsNum - 1)
	{
		m_currentSector++;
		updateSliderPos();
		addSliderCommand();
	}
}
void SliderElementClass::backSector()
{
	CommandClass* command;
	if (m_currentSector > 0)
	{
		m_currentSector--;
		updateSliderPos();
		addSliderCommand();
	}
}


//Actions on click
std::string SliderElementClass::pick(int posX, int posY)
{
	if (isVisible())
	{
		for (int i = m_bitmapsNum - 1; i >= 0; i--)
		{
			//check if pick in rectangle of bitmap
			if (m_bitmapsInfo[i].visible && (m_bitmapsInfo[i].posX < posX && m_bitmapsInfo[i].posX + m_bitmapsInfo[i].width > posX) &&
				(m_bitmapsInfo[i].posY < posY && m_bitmapsInfo[i].posY + m_bitmapsInfo[i].height > posY))
			{
				m_lastX = posX;
				m_lastY = posY;
				return m_bitmapsInfo[i].name;
			}
		}
	}
	return "";
}
void SliderElementClass::addSelCommand(const std::string& name)
{
	int i = findBMbyName(name);

	this->addSelCommand(name);

	CommandManagerClass::getI().addChange("NUM" + m_name, m_currentSector);

}
void SliderElementClass::addUnSelCommand(const std::string& name)
{
	int i = findBMbyName(name);

	this->addUnSelCommand(name);

	CommandManagerClass::getI().addChange("NUM" + m_name, m_currentSector);
}
void SliderElementClass::addPickCommand(const std::string& name)
{
	int i = findBMbyName(name);

	//start drag slider in any case
	if (name == "slider")
		m_dragType = 1;
	else
		m_dragType = 0;

	this->addSelCommand(name);

	CommandManagerClass::getI().addChange("NUM" + m_name, m_currentSector);

}
void SliderElementClass::addUnPickCommand(const std::string& name)
{
	int i = findBMbyName(name);



	if (name == "backLine")
	{
		updateSector(m_lastX, m_lastY);
	}

	this->addUnPickCommand(name);

	CommandManagerClass::getI().addChange("NUM" + m_name, m_currentSector);

}
void SliderElementClass::addSliderCommand()
{
	CommandManagerClass::getI().addCommand(m_sliderCommand, m_actionsFileName);

	CommandManagerClass::getI().addChange("NUM" + m_name, m_currentSector);
}

//work with files
bool SliderElementClass::readFromFile(const std::string& filename)
{
	std::fstream file;
	file.open(filename, std::ios::in);
	std::string temp; // "trash"
	file >> temp >> temp >> m_name; // Getting name of element
	file >> temp >> temp >> m_bitmapsNum; // Getting number of bitmaps
	file >> temp >> temp >> m_textsNum; // Getting number of texts
	file >> temp >> temp >> m_sectorsNum; //  Getting number of sectors
	file >> temp >> temp >> temp; //  Getting orientation of slider
	if (temp == "horizontal")
		m_orientation = 1;
	else
		m_orientation = 0;
	file >> temp >> temp >> m_sliderCommand; // Getting command for slider
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