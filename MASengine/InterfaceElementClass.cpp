///////////////////////////////////////////////////////////////////////////////
// Filename: InterfaceElementClass.cpp
///////////////////////////////////////////////////////////////////////////////


#include"InterfaceElementClass.h"

InterfaceElementClass::InterfaceElementClass()
{
	m_texts = 0;
	m_bitmaps = 0;
	m_textsInfo = 0;
	m_bitmapsInfo = 0;

}
InterfaceElementClass::InterfaceElementClass(const InterfaceElementClass&)
{

}
InterfaceElementClass::~InterfaceElementClass()
{

}


bool InterfaceElementClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, const std::string& filename, int screenWidth, int screenHeight)
{

	bool result;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	// Store the screen width and height.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	result = readFromFile(ModManagerClass::getI().getDirectory(ModManagerClass::getI().getHash(filename)) + filename); // Loading params from file
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 13-1");
		return false;
	}

	//Loading bitmaps
	m_bitmaps = new(1) BitmapClass*[m_bitmapsNum];
	if (!m_bitmaps)
		return false;

	for (int i = 0; i < m_bitmapsNum; i++)
	{
		m_bitmaps[i] = new(1) BitmapClass;
		if (!m_bitmaps[i])
			return false;
		//Initialization
		result = m_bitmaps[i]->Initialize(device, screenWidth, screenHeight, m_bitmapsInfo[i].filename,
			m_bitmapsInfo[i].width, m_bitmapsInfo[i].height);
		if (!result)
		{
			LogManagerClass::getI().addLog("Error 13-2");
			return false;
		}
	}

	//Loading texts
	m_texts = new(1) TextClass*[m_textsNum];
	if (!m_texts)
		return false;

	for (int i = 0; i < m_textsNum; i++)
	{
		m_texts[i] = new(1) TextClass;
		if (!m_texts[i])
			return false;

		//Initialization
		result = m_texts[i]->Initialize(device, deviceContext, hwnd, screenWidth, screenHeight, m_textsInfo[i].stringsNum,
			m_textsInfo[i].maxLength, m_textsInfo[i].orientation, m_textsInfo[i].fontFilename);
		if (!result)
		{
			LogManagerClass::getI().addLog("Error 13-3");
			return false;
		}

		for (int j = 0; j < m_textsInfo[i].stringsNum; j++)
		{
			// Filling of strings
			result = updateSentence(deviceContext, i, j);
			if (!result)
			{
				LogManagerClass::getI().addLog("Error 13-4");
				return false;
			}
		}
	}


	return true;
}

void InterfaceElementClass::Shutdown()
{
	if (m_texts)
	{
		// Release all texts.
		for (int i = 0; i < m_textsNum; i++)
		{
			if (m_texts[i])
			{
				m_texts[i]->Shutdown();
				::operator delete[](m_texts[i], sizeof(TextClass), 1);
				m_texts[i] = 0;
			}
		}
	
		::operator delete[](m_texts, sizeof(TextClass*) * m_textsNum, 1);
		m_texts = 0;
	}
	if (m_bitmaps)
	{
		// Release all bitmaps
		for (int i = 0; i < m_bitmapsNum; i++)
		{
			if (m_bitmaps[i])
			{
				m_bitmaps[i]->Shutdown();
				::operator delete[](m_bitmaps[i], sizeof(BitmapClass), 1);
				m_bitmaps[i] = 0;
			}
		}
	
		::operator delete[](m_bitmaps, sizeof(BitmapClass*) * m_bitmapsNum, 1);
		m_bitmaps = 0;
	}

	// Release all sentence info.
	for (int i = 0; i < m_textsNum; i++)
	{
		if (m_textsInfo[i].strings)
		{
			::operator delete[](m_textsInfo[i].strings, sizeof(StringInfo) * m_textsInfo[i].stringsNum, 1);
			m_textsInfo[i].strings = 0;
		}
	}

	//delete info
	if (m_textsInfo)
	{
		::operator delete[](m_textsInfo, sizeof(TextInfo)* m_textsNum, 1);
		m_textsInfo = 0;
	}
	if (m_bitmapsInfo)
	{
		::operator delete[](m_bitmapsInfo, sizeof(BitmapInfo)* m_bitmapsNum, 1);
		m_bitmapsInfo = 0;
	}
	

	return;


}

bool InterfaceElementClass::Render(FontShaderClass* fontShader, InterfaceShaderClass* interfaceShader, ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
	D3DXMATRIX orthoMatrix, D3DXMATRIX baseViewMatrix)
{
	bool result;

	// Render  all bitmaps
	for (int i = 0; i < m_bitmapsNum; i++)
	{
		if (m_bitmapsInfo[i].visible)
		{
			result = m_bitmaps[i]->Render(deviceContext, m_bitmapsInfo[i].posX, m_bitmapsInfo[i].posY);
			if (!result)
			{
				return false;
			}

			result = interfaceShader->Render(deviceContext, m_bitmaps[i]->GetIndexCount(), worldMatrix, baseViewMatrix, orthoMatrix, m_bitmaps[i]->GetTexture(),
				m_bitmapsInfo[i].vertPercent, m_bitmapsInfo[i].horPercent, m_bitmapsInfo[i].selCol,
				m_bitmapsInfo[i].selIntensivity, m_bitmapsInfo[i].transparency);
			if (!result)
			{
				return false;
			}

		}
	}

	// Render all texts.
	for (int i = 0; i < m_textsNum; i++)
	{
		if (m_textsInfo[i].visible)
		{
			result = m_texts[i]->Render(fontShader, deviceContext, worldMatrix, orthoMatrix, baseViewMatrix);
			if (!result)
			{
				return false;
			}
		}
	}

	return true;
}

//Getting element`s data
std::string InterfaceElementClass::getName()
{
	return m_name;
}
bool InterfaceElementClass::isVisible()
{
	return m_visible;
}

//Updating element`s data
void InterfaceElementClass::setVisible(bool visible)
{
	CommandClass* command = new CommandClass;
	for (int i = 0; i < m_bitmapsNum; i++)
	{
		CommandManagerClass::getI().addCommand(m_bitmapsInfo[i].unSelAction, m_actionsFileName);
	}
	m_visible = visible;
}
void InterfaceElementClass::updateDrag(ID3D11DeviceContext* deviceContext, int posX, int posY)
{
	int Xdrag = posX - m_lastX;
	int Ydrag = posY - m_lastY;

	m_lastX = posX;
	m_lastY = posY;

	setPos(deviceContext, m_posX + Xdrag, m_posY + Ydrag);
}
void InterfaceElementClass::setPos(ID3D11DeviceContext* deviceContext, int posX, int posY)
{
	int Xdrag = posX - m_posX;
	int Ydrag = posY - m_posY;

	m_posX += Xdrag;
	m_posY += Ydrag;

	for (int i = 0; i < m_bitmapsNum; i++)
	{
		updateBMposX(m_bitmapsInfo[i].name, m_bitmapsInfo[i].posX + Xdrag);
		updateBMposY(m_bitmapsInfo[i].name, m_bitmapsInfo[i].posY + Ydrag);
	}
	for (int i = 0; i < m_textsNum; i++)
	{
		for (int j = 0; j < m_textsInfo[i].stringsNum; j++)
		{
			updateTSposX(deviceContext,m_textsInfo[i].name, j, m_textsInfo[i].strings[j].posX + Xdrag);
			updateTSposY(deviceContext,m_textsInfo[i].name, j, m_textsInfo[i].strings[j].posY + Ydrag);
		}
	}
}
void InterfaceElementClass::endDrag()
{

}
void InterfaceElementClass::startDrag(int posX, int posY)
{
	m_lastX = posX;
	m_lastY = posY;
}

//Finding index of element ny his name
int InterfaceElementClass::findBMbyName(const std::string& name)
{
	for (int i = 0; i < m_bitmapsNum; i++)
	{
		if (m_bitmapsInfo[i].name == name)
			return i;
	}
	return 0;
}
int InterfaceElementClass::findTbyName(const std::string& name)
{
	for (int i = 0; i < m_textsNum; i++)
	{
		if (m_textsInfo[i].name == name)
			return i;
	}
	return 0;
}

//Updating params of bitmaps
void  InterfaceElementClass::setBMvisible(const std::string& name, bool visible)
{
	int i = findBMbyName(name);
	if (i < m_bitmapsNum)
	{
		m_bitmapsInfo[i].visible = visible;
		CommandManagerClass::getI().addCommand(m_bitmapsInfo[i].unSelAction, m_actionsFileName);
	}
}
void InterfaceElementClass::updateBMposX(const std::string& name, int posX)
{
	int i = findBMbyName(name);
	if (i < m_bitmapsNum)
		m_bitmapsInfo[i].posX = posX;
}
void InterfaceElementClass::updateBMposY(const std::string& name, int posY)
{
	int i = findBMbyName(name);
	if (i < m_bitmapsNum)
		m_bitmapsInfo[i].posY = posY;
}
void InterfaceElementClass::updateBMvertPercent(const std::string& name, float vertPercent)
{
	int i = findBMbyName(name);
	if (i < m_bitmapsNum)
		m_bitmapsInfo[i].vertPercent = vertPercent;
}
void InterfaceElementClass::updateBMhorPercent(const std::string& name, float horPercent)
{
	int i = findBMbyName(name);
	if (i < m_bitmapsNum)
		m_bitmapsInfo[i].horPercent = horPercent;
}
void InterfaceElementClass::updateBMtranspar(const std::string& name, float transparency)
{
	int i = findBMbyName(name);
	if (i < m_bitmapsNum)
		m_bitmapsInfo[i].transparency = transparency;
}
void InterfaceElementClass::updateBMselIntens(const std::string& name, float selIntensivity)
{
	int i = findBMbyName(name);
	if (i < m_bitmapsNum)
		m_bitmapsInfo[i].selIntensivity = selIntensivity;
}
void InterfaceElementClass::updateBMselCol(const std::string& name, D3DXVECTOR4 selCol)
{
	int i = findBMbyName(name);
	if (i < m_bitmapsNum)
		m_bitmapsInfo[i].selCol = selCol;
}

//Updating params of strings
void  InterfaceElementClass::setTvisible(const std::string& name, bool visible)
{
	int i = findTbyName(name);
	if (i < m_textsNum)
		m_textsInfo[i].visible = visible;
}
bool InterfaceElementClass::updateTSposY(ID3D11DeviceContext* deviceContext, const std::string& name, int ind, int posY)
{
	bool result;

	int i = findTbyName(name);
	if (i < m_textsNum)
		if (ind < m_textsInfo[i].stringsNum)
		{
			m_textsInfo[i].strings[ind].posY = posY;

			result = updateSentence(deviceContext, i, ind);
			if (!result)
				return false;
		}
}
bool InterfaceElementClass::updateTSposX(ID3D11DeviceContext* deviceContext, const std::string& name, int ind, int posX)
{
	bool result;
	int i = findTbyName(name);
	if (i < m_textsNum)
		if (ind < m_textsInfo[i].stringsNum)
		{
			m_textsInfo[i].strings[ind].posX = posX;
			
			result = updateSentence(deviceContext, i, ind);
			if (!result)
				return false;
		}
}
bool InterfaceElementClass::updateTStext(ID3D11DeviceContext* deviceContext, const std::string& name, int ind, const std::string& text)
{
	bool result;

	int i = findTbyName(name);
	if (i < m_textsNum)
		if (ind < m_textsInfo[i].stringsNum)
		{
			m_textsInfo[i].strings[ind].text = text;
			
			result = updateSentence(deviceContext, i, ind);
			if (!result)
				return false;
		}
}
bool InterfaceElementClass::updateTScolor(ID3D11DeviceContext* deviceContext, const std::string& name, int ind, D3DXVECTOR4 color)
{
	bool result;
	int i = findTbyName(name);
	if (i < m_textsNum)
		if (ind < m_textsInfo[i].stringsNum)
		{
			m_textsInfo[i].strings[ind].color = color;

			result = updateSentence(deviceContext, i, ind);
			if (!result)
				return false;
		}
}
bool InterfaceElementClass::updateTSadding(ID3D11DeviceContext* deviceContext, const std::string& name, int ind, const std::string& adding)
{
	bool result;

	int i = findTbyName(name);
	if (i < m_textsNum)
		if (ind < m_textsInfo[i].stringsNum)
		{
			m_textsInfo[i].strings[ind].adding = adding;

			result = updateSentence(deviceContext, i, ind);
			if (!result)
				return false;
		}
}

//Getting data about strings
int InterfaceElementClass::getTSsize(const std::string& name, int ind)
{
	int i = findTbyName(name);
	if (i < m_textsNum)
	{
		if (ind < m_textsInfo[i].stringsNum)
			return m_textsInfo[i].strings[ind].text.size();
		else
			return 0;
	}
	else
		return 0;

}
std::string InterfaceElementClass::getTStext(const std::string& name, int ind)
{
	int i = findTbyName(name);
	if (i < m_textsNum)
		if (ind < m_textsInfo[i].stringsNum)
			return m_textsInfo[i].strings[ind].text;
}

//Actions on click
std::string InterfaceElementClass::pick(int posX, int posY)
{
	if (isVisible())
	{
		for (int i = m_bitmapsNum - 1; i >= 0; i--)
		{
			//check if pick in rectangle of bitmap
			if (m_bitmapsInfo[i].visible && (m_bitmapsInfo[i].posX < posX && m_bitmapsInfo[i].posX + m_bitmapsInfo[i].width > posX) &&
				(m_bitmapsInfo[i].posY < posY && m_bitmapsInfo[i].posY + m_bitmapsInfo[i].height > posY))
			{
				return m_bitmapsInfo[i].name;
			}
		}
	}
	return "";
}
void InterfaceElementClass::addSelCommand(const std::string& name)
{
	int i = findBMbyName(name);

	CommandManagerClass::getI().addCommand(m_bitmapsInfo[i].selAction, m_actionsFileName);
}
void InterfaceElementClass::addUnSelCommand(const std::string& name)
{
	int i = findBMbyName(name);

	CommandManagerClass::getI().addCommand(m_bitmapsInfo[i].unSelAction, m_actionsFileName);
}
void InterfaceElementClass::addPickCommand(const std::string& name)
{
	int i = findBMbyName(name);
	
	CommandManagerClass::getI().addCommand(m_bitmapsInfo[i].pickAction, m_actionsFileName);
}
void InterfaceElementClass::addUnPickCommand(const std::string& name)
{
	int i = findBMbyName(name);
	
	CommandManagerClass::getI().addCommand(m_bitmapsInfo[i].unPickAction, m_actionsFileName);
}
void InterfaceElementClass::setSelCommand(const std::string& name, const std::string& action)
{
	int i = findBMbyName(name);
	m_bitmapsInfo[i].selAction = action;
}
void InterfaceElementClass::setUnSelCommand(const std::string& name, const std::string& action)
{
	int i = findBMbyName(name);
	m_bitmapsInfo[i].unSelAction = action;
}
void InterfaceElementClass::setPickCommand(const std::string& name, const std::string& action)
{
	int i = findBMbyName(name);
	m_bitmapsInfo[i].pickAction = action;
}
void InterfaceElementClass::setUnPickCommand(const std::string& name, const std::string& action)
{
	int i = findBMbyName(name);
	m_bitmapsInfo[i].unPickAction = action;
}


//CommandClass * InterfaceElementClass::makeChanges(CommandClass * command, int num)
//{
//	command->addChange("screenHeight", m_screenHeight);
//	command->addChange("screenWidth", m_screenWidth);
//	command->makeChanges();
//	return command;
//}



///PRIVATE///

//Work with files
bool InterfaceElementClass::readFromFile(const std::string& filename)
{
	std::fstream file;
	file.open(filename, std::ios::in);
	if (file.fail())
	{
		LogManagerClass::getI().addLog("Error 13-6");
		return false;
	}


	std::string temp; // "trash"
	file >> temp >> temp >> m_name; // Getting name of element
	file >> temp >> temp >> m_bitmapsNum; // Getting number of bitmaps
	file >> temp >> temp >> m_textsNum; // Getting number of texts
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

bool InterfaceElementClass::readBitmapsInfoFromFile(std::fstream* file)
{
	std::string temp;

	// Creating array with bitmaps data
	m_bitmapsInfo = new(1) BitmapInfo[m_bitmapsNum];
	if (!m_bitmapsInfo)
		return false;

	*file >> temp >> temp; // title and {

	for (int i = 0; i < m_bitmapsNum; i++)
	{
		*file >> temp >> temp; // title and {

		//name of element
		*file >> temp >> temp >> m_bitmapsInfo[i].name;

		//name of file
		*file >> temp >> temp >> m_bitmapsInfo[i].filename;

		//width
		*file >> temp >> temp >> temp;
		if (temp == "screenWidth")
			m_bitmapsInfo[i].width = m_screenWidth;
		else
			m_bitmapsInfo[i].width = atoi(temp.c_str());

		//height
		*file >> temp >> temp >> temp;
		if (temp == "screenHeight")
			m_bitmapsInfo[i].height = m_screenHeight;
		else
			m_bitmapsInfo[i].height = atoi(temp.c_str());

		//position on X
		*file >> temp >> temp >> temp;
		if (temp == "middle")
			m_bitmapsInfo[i].posX = (m_screenWidth - m_posX) / 2 - m_bitmapsInfo[i].width / 2;
		else if (temp == "screenWidth")
			m_bitmapsInfo[i].posX = m_screenWidth + m_posX;
		else
			m_bitmapsInfo[i].posX = atoi(temp.c_str()) + m_posX;
		if (m_horOrien)
			m_bitmapsInfo[i].posX = m_screenWidth - m_bitmapsInfo[i].posX;

		//position on Y
		*file >> temp >> temp >> temp;
		if (temp == "middle")
			m_bitmapsInfo[i].posY = (m_screenHeight - m_posY) / 2 - m_bitmapsInfo[i].height / 2;
		else if (temp == "screenHeight")
			m_bitmapsInfo[i].posY = m_screenHeight + m_posY;
		else
			m_bitmapsInfo[i].posY = atoi(temp.c_str()) + m_posY;
		if (m_vertOrien)
			m_bitmapsInfo[i].posY = m_screenHeight - m_bitmapsInfo[i].posY;

		//actions
		*file >> temp >> temp >> m_bitmapsInfo[i].selAction;
		*file >> temp >> temp >> m_bitmapsInfo[i].unSelAction;
		*file >> temp >> temp >> m_bitmapsInfo[i].pickAction;
		*file >> temp >> temp >> m_bitmapsInfo[i].unPickAction;
		*file >> temp; // }
	}

	*file >> temp; // }

	return true;
}

bool InterfaceElementClass::readTextsInfoFromFile(std::fstream* file)
{
	std::string temp;

	m_textsInfo = new(1) TextInfo[m_textsNum];
	if (!m_textsInfo)
		return false;

	*file >> temp >> temp; // title and   {

	for (int i = 0; i < m_textsNum; i++)
	{
		*file >> temp >> temp; //   title and {
		*file >> temp >> temp >> m_textsInfo[i].name;
		*file >> temp >> temp >> m_textsInfo[i].fontFilename;
		*file >> temp >> temp >> m_textsInfo[i].textFilename;
		m_textsInfo[i].textFilename.replace(m_textsInfo[i].textFilename.find_first_of("XXX___"), 6, SettingsClass::getI().getStrParameter("Language")); ///rework
		*file >> temp >> temp >> m_textsInfo[i].maxLength;
		*file >> temp >> temp >> m_textsInfo[i].stringsNum;

		//text orientation
		*file >> temp >> temp >> temp;
		if (temp == "MIDDLE")
			m_textsInfo[i].orientation = 1;
		else if (temp == "RIGHT")
			m_textsInfo[i].orientation = 2;
		else
			m_textsInfo[i].orientation = 0;

		//strings
		m_textsInfo[i].strings = new(1) StringInfo[m_textsInfo[i].stringsNum];
		if (!m_textsInfo[i].strings)
			return false;

		for (int j = 0; j < m_textsInfo[i].stringsNum; j++)
		{
			*file >> temp >> temp; //   title and {

			//add text
			*file >> temp >> temp >> temp; 
			m_textsInfo[i].strings[j].text = temp;
			TextManagerClass::getI().addText(temp, m_textsInfo[i].textFilename);

			//sentence params
			*file >> temp >> temp >> m_textsInfo[i].strings[j].maxWidth;
			*file >> temp >> temp >> m_textsInfo[i].strings[j].size;
			*file >> temp >> temp >> m_textsInfo[i].strings[j].color.x >> m_textsInfo[i].strings[j].color.y >> m_textsInfo[i].strings[j].color.z >> m_textsInfo[i].strings[j].color.w;

			//position on X
			*file >> temp >> temp >> temp;
			if (temp == "middle")
				m_textsInfo[i].strings[j].posX = (m_screenWidth - m_posX) / 2 - m_textsInfo[i].strings[j].maxWidth / 2;
			else
				m_textsInfo[i].strings[j].posX = atoi(temp.c_str()) + m_posX;
			if (m_horOrien)
			{
				m_textsInfo[i].strings[j].posX = m_screenWidth - m_textsInfo[i].strings[j].posX;
			}

			//position on Y
			*file >> temp >> temp >> temp;
			if (temp == "middle")
				m_textsInfo[i].strings[j].posY = (m_screenHeight - m_posY) / 2;
			else
				m_textsInfo[i].strings[j].posY = atoi(temp.c_str()) + m_posY;
			if (m_vertOrien)
			{
				m_textsInfo[i].strings[j].posY = m_screenHeight - m_textsInfo[i].strings[j].posY;
			}
			*file >> temp; // }
		}
		*file >> temp; // }
	}
	*file >> temp; // }

	return true;
}

bool InterfaceElementClass::updateSentence(ID3D11DeviceContext* deviceContext, int textInd, int sentenceInd)
{
	bool result;

	result = m_texts[textInd]->UpdateSentence(deviceContext, sentenceInd, 
		TextManagerClass::getI().getText(m_textsInfo[textInd].strings[sentenceInd].text, m_textsInfo[textInd].textFilename) + m_converter.from_bytes(m_textsInfo[textInd].strings[sentenceInd].adding),
		m_textsInfo[textInd].strings[sentenceInd].posX, m_textsInfo[textInd].strings[sentenceInd].posY, m_textsInfo[textInd].strings[sentenceInd].size,
		m_textsInfo[textInd].strings[sentenceInd].maxWidth, m_textsInfo[textInd].strings[sentenceInd].color);
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 13-5");
		return false;
	}
}