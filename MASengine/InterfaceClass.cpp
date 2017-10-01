////////////////////////////////////////////////////////////////////////////////
// Filename: interfaceclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "interfaceclass.h"


InterfaceClass::InterfaceClass()
{
	m_interfaceElements = 0;
	m_calculator = 0;
}
InterfaceClass::InterfaceClass(const InterfaceClass&)
{

}
InterfaceClass::~InterfaceClass()
{

}

bool InterfaceClass::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, HWND hwnd, int screenWidth, int screenHeight)
{
	bool result;
	std::ifstream elementsFile;

	//save screen params
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	//Init calculator
	m_calculator = new(1) CalculatorClass;
	if (!m_calculator)
		return false;

	//Open file with elements
	elementsFile.open(SettingsClass::getI().getStrParameter("ElementsFilename"));
	if (elementsFile.fail())
	{
		LogManagerClass::getI().addLog("Error 14-1");
		return false;
	}

	elementsFile >> m_elementsNum;
	
	
	std::string elementType;

	//init all interface elements
	m_interfaceElements = new(1) InterfaceElementClass*[m_elementsNum];
	if (!m_interfaceElements)
		return false;
	for (int i = 0; i < m_elementsNum; i++)
	{
		PathClass* elementFilename = new(4) PathClass;
		elementsFile >> elementFilename;

		//create different types of elements
		elementsFile >> elementType;
		if (elementType == "slider")
		{
			m_interfaceElements[i] = new(1) SliderElementClass;
		}
		else if (elementType == "list")
		{
			m_interfaceElements[i] = new(1) ListElementClass;
		}
		else if (elementType == "group")
		{
			m_interfaceElements[i] = new(1) GroupElementClass;
		}
		else
		{
			m_interfaceElements[i] = new(1) InterfaceElementClass;
		}
		result = m_interfaceElements[i]->Initialize(device, deviceContext, hwnd, elementFilename, m_screenWidth, m_screenHeight);
		if(!result)
		{
			LogManagerClass::getI().addLog("Error 14-2");
			return false;
		}

		//set new params in loadscreen
		LoadScreenManagerClass::getI().changeLine(m_interfaceElements[i]->getName()+"_init", 0.05f + 0.15f*(i/m_elementsNum));
		
	}

	return true;
}

void InterfaceClass::Shutdown()
{
	// Release interface elements
	for (int i = 0; i < m_elementsNum; i++)
	{
		if (m_interfaceElements[i])
		{
			m_interfaceElements[i]->Shutdown();
			::operator delete(m_interfaceElements[i], sizeof(*m_interfaceElements[i]), 1);
			m_interfaceElements[i] = 0;
		}
	}
	::operator delete[](m_interfaceElements, sizeof(*m_interfaceElements)*m_elementsNum, 1);

	if (m_calculator)
	{
		::operator delete(m_calculator, sizeof(CalculatorClass), 1);
		m_calculator = 0;
	}
}

bool InterfaceClass::Frame(ID3D11DeviceContext* deviceContext, int mouseX, int mouseY)
{

	if (isElVisible("Test"))
	{
		//m_TestElement->updateBMposX("tramp", rand() % m_screenWidth - 742);
		//m_TestElement->updateBMposY("tramp", rand() % (m_screenWidth - 400));
		//m_TestElement->updateTSposY("maga", 1, rand() % (m_screenWidth - 100));
		//m_TestElement->updateTSposX("maga", 1, rand() % (m_screenWidth - 100));
		//m_TestElement->updateTStext("maga", 1, rand() % m_TestElement->GetTSsize("maga", 1), rand() % m_TestElement->GetTSsize("maga", 1), "qa");
		updateElTScolor(deviceContext, "Test", "maga", 0, D3DXVECTOR4((float)rand() / (float)(RAND_MAX), (float)rand() / (float)(RAND_MAX), (float)rand() / (float)(RAND_MAX),1));
		updateElTScolor(deviceContext, "Test", "maga", 1, D3DXVECTOR4((float)rand() / (float)(RAND_MAX), (float)rand() / (float)(RAND_MAX), (float)rand() / (float)(RAND_MAX),1));
		updateElTScolor(deviceContext, "Test", "maga", 2, D3DXVECTOR4((float)rand() / (float)(RAND_MAX), (float)rand() / (float)(RAND_MAX), (float)rand() / (float)(RAND_MAX),1));
	}

	//Updating coordinates of cursor
	if (isElVisible("Cursor"))
	{
		updateElBMposX("Cursor", "cursor", mouseX);
		updateElBMposY("Cursor", "cursor", mouseY);
	}
	// first check for LBM
	if (!m_LBMDown)
		CheckSelection(deviceContext, mouseX, mouseY);


	updateDrag(deviceContext, mouseX, mouseY);

	m_mouseX = mouseX;
	m_mouseY = mouseY;

	return true;
}

bool InterfaceClass::Render(InterfaceShaderClass* interfaceShader, FontShaderClass* fontShader,
	ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX orthoMatrix)
{
	bool result;

	for (int i = 0; i < m_elementsNum; i++)
	{

		if (m_interfaceElements[i] && m_interfaceElements[i]->isVisible())
		//if (m_interfaceElements[i])
		{
			result = m_interfaceElements[i]->Render(fontShader, interfaceShader, deviceContext, worldMatrix, orthoMatrix, viewMatrix);
			if (!result)
			{
				LogManagerClass::getI().addLog("Error 14-3");
				return false;
			}
		}

	}

	return true;
}

//update data of interface
void InterfaceClass::setElVisible(const std::string& elname, bool visible)
{
	InterfaceElementClass* element = findElbyName(elname);
	if (element)
	{
		element->setVisible(visible);
	}

}

void InterfaceClass::updateDrag(ID3D11DeviceContext* deviceContext, int posX, int posY)
{
	for (int i = 0; i < m_dragElements.size(); i++)
	{
		InterfaceElementClass* element = findElbyName(m_dragElements[i]);
		if (element)
		{
			element->updateDrag(deviceContext, posX, posY);
		}
	}

}
void InterfaceClass::startDrag(const std::string& elname)
{
	InterfaceElementClass* element = findElbyName(elname);
	if (element)
	{
		m_dragElements.emplace_back(elname);
		element->startDrag(m_mouseX, m_mouseY);
	}
}
void InterfaceClass::endDrag(const std::string& elname)
{
	InterfaceElementClass* element = findElbyName(elname);
	for (int i = 0; i < m_dragElements.size(); i++)
	{
		if (m_dragElements[i] == elname)
		{
			m_dragElements.erase(m_dragElements.begin() + i);
			if (element)
				element->endDrag();
			break;
		}
	}
}
void InterfaceClass::setPos(ID3D11DeviceContext* deviceContext,const std::string& elname, int posX, int posY)
{
	InterfaceElementClass* element = findElbyName(elname);
	if (element)
		element->setPos(deviceContext, posX, posY);
}



//Updating params of bitmaps
void InterfaceClass::setElBMvisible(const std::string& elname, const std::string& bmname, bool visible)
{
	InterfaceElementClass* element = findElbyName(elname);
	if (element)
	{
		element->setBMvisible(bmname, visible);
	}
}
void InterfaceClass::updateElBMposX(const std::string& elname, const std::string& bmname, int posX)
{
	InterfaceElementClass* element = findElbyName(elname);
	if (element)
		element->updateBMposX(bmname, posX);
}
void InterfaceClass::updateElBMposY(const std::string& elname, const std::string& bmname, int posY)
{
	InterfaceElementClass* element = findElbyName(elname);
	if (element)
		element->updateBMposY(bmname, posY);
}
void InterfaceClass::updateElBMvertPercent(const std::string& elname, const std::string& bmname, float vertPercent)
{
	InterfaceElementClass* element = findElbyName(elname);
	if (element)
		element->updateBMvertPercent(bmname, vertPercent);
}
void InterfaceClass::updateElBMhorPercent(const std::string& elname, const std::string& bmname, float horPercent)
{
	InterfaceElementClass* element = findElbyName(elname);
	if (element)
		element->updateBMhorPercent(bmname, horPercent);
}
void InterfaceClass::updateElBMtranspar(const std::string& elname, const std::string& bmname, float transparency)
{
	InterfaceElementClass* element = findElbyName(elname);
	if (element)
		element->updateBMtranspar(bmname, transparency);
}
void InterfaceClass::updateElBMselIntens(const std::string& elname, const std::string& bmname, float selIntensivity)
{
	InterfaceElementClass* element = findElbyName(elname);
	if (element)
		element->updateBMselIntens(bmname, selIntensivity);
}
void InterfaceClass::updateElBMselCol(const std::string& elname, const std::string& bmname, D3DXVECTOR4 selCol)
{
	InterfaceElementClass* element = findElbyName(elname);
	if (element)
		element->updateBMselCol(bmname, selCol);
}
void InterfaceClass::setBMCommand(const std::string& elname, const std::string& bmname, const std::string& commandType, const std::string& command)
{
	InterfaceElementClass* element = findElbyName(elname);
	if (element)
	{
		if (commandType == "sel")
			element->setSelCommand(bmname, command);
		else if (commandType == "unSel")
			element->setUnSelCommand(bmname, command);
		else if (commandType == "pick")
			element->setPickCommand(bmname, command);
		else if (commandType == "unPick")
			element->setUnPickCommand(bmname, command);
	}
}

bool InterfaceClass::setNewBM(ID3D11Device* device, const std::string & elname, const std::string & bmname, PathClass* filename)
{
	bool result;
	InterfaceElementClass* element = findElbyName(elname);
	if (element)
	{
		result = element->setNewBM(device, bmname, filename);
		if (!result)
			return false;
	}
	return true;
}

//Updating params of strings
void InterfaceClass::setElTvisible(const std::string& elname, const std::string& tname, bool visible)
{
	InterfaceElementClass* element = findElbyName(elname);
	if (element)
		element->setTvisible(tname, visible);
}
bool InterfaceClass::updateElTSposY(ID3D11DeviceContext* deviceContext, const std::string& elname, const std::string& tname, int ind, int posY)
{
	InterfaceElementClass* element = findElbyName(elname);
	if (element)
		return element->updateTSposY(deviceContext,tname, ind, posY);
}
bool InterfaceClass::updateElTSposX(ID3D11DeviceContext* deviceContext, const std::string& elname, const std::string& tname, int ind, int posX)
{
	InterfaceElementClass* element = findElbyName(elname);
	if (element)
		return element->updateTSposX(deviceContext,tname, ind, posX);
}
bool InterfaceClass::updateElTStext(ID3D11DeviceContext* deviceContext, const std::string& elname, const std::string& tname, int ind, const std::string& text)
{
	InterfaceElementClass* element = findElbyName(elname);
	if (element)
		return element->updateTStext(deviceContext,tname, ind,  text);
}
bool InterfaceClass::updateElTScolor(ID3D11DeviceContext* deviceContext, const std::string& elname, const std::string& tname, int ind, D3DXVECTOR4 color)
{
	InterfaceElementClass* element = findElbyName(elname);
	if (element)
		return element->updateTScolor(deviceContext,tname, ind, color);
}
bool InterfaceClass::updateElTSadding(ID3D11DeviceContext* deviceContext, const std::string& elname, const std::string& tname, int ind, const std::string& text)
{
	InterfaceElementClass* element = findElbyName(elname);
	if (element)
		return element->updateTSadding(deviceContext,tname, ind, text);
}

void InterfaceClass::nextSector(const std::string & elname)
{
	InterfaceElementClass* element = findElbyName(elname);

	if (element)
	{
		((SliderElementClass*)element)->nextSector();
	}

}

void InterfaceClass::backSector(const std::string & elname)
{
	InterfaceElementClass* element = findElbyName(elname);

	if (element)
	{
		((SliderElementClass*)element)->backSector();
	}
}

bool InterfaceClass::addElement(ID3D11Device * device, ID3D11DeviceContext * deviceContext, HWND hwnd, const std::string & elname)
{
	bool result;

	InterfaceElementClass* element = findElbyName(elname);

	if (element)
	{
		result = element->addElement(device,deviceContext,hwnd);
		if (!result)
		{
			return false;
		}
	}

	return true;
}

void InterfaceClass::deleteElement(ID3D11DeviceContext* deviceContext, const std::string & elname, int ind)
{
	InterfaceElementClass* element = findElbyName(elname);
	if (element)
		element->deleteElement(deviceContext, ind);
}

void InterfaceClass::setLastElement(ID3D11DeviceContext * deviceContext, const std::string & elname, int ind)
{
	InterfaceElementClass* element = findElbyName(elname);
	if (element)
		((ListElementClass*)element)->setLastElement(deviceContext,ind);
}

void InterfaceClass::clear(const std::string & elname)
{
	InterfaceElementClass* element = findElbyName(elname);
	if (element)
		element->clear();
}


///Finding index of element by his name
InterfaceElementClass* InterfaceClass::findElbyName(const std::string& name)
{
	int num = -1; // if -1, this is normal element
	int ind = -1;
	std::string subname = name;
	InterfaceElementClass* element;

	//erase indexes in name
	if (name.size() > 0 && name[0] == '_')
		subname.erase(0, subname.find_last_of("_") + 1);

	// find index of main element
	for (int i = 0; i < m_elementsNum; i++)
	{
		if (m_interfaceElements[i]->getName() == subname)
		{
			ind = i;
			break;
		}
	}

	//return nothing is this is eleent is not exist
	if (ind < 0)
		return NULL;

	element = m_interfaceElements[ind];

	//get subelements if it is in list
	subname = name;
	while (subname.size() > 0 && subname[0] == '_')
	{
		num = getNumFromName(subname);
		subname.erase(0, 1);
		subname.erase(0, subname.find_first_of("_") + 1);

	//if subelement of list
	if (num >= 0)
		element = element->getElement(num);
	else
		return NULL;
	}

	return element;
}

//Getting data about elements
bool InterfaceClass::getEl(int posX, int posY, int& ind,  std::string& name)
{
	for (int i = m_elementsNum - 1; i >= 0; i--)
	{
		if (m_interfaceElements[i]->getName() != "Cursor")
		{
			name = m_interfaceElements[i]->pick(posX, posY);
			if (name.size() > 0)
			{
				ind = i;
				return true;
			}
		}
	}
	ind = 0;
	if (m_LBMDown)
		return true;
	else
		return false;
}
int InterfaceClass::getElTSsize(const std::string& elname, const std::string& tname, int ind)
{
	InterfaceElementClass* element = findElbyName(elname);
	if (element)
		return element->getTSsize(tname, ind);
	else
		return 0;
}
bool InterfaceClass::isElVisible(const std::string& elname)
{
	InterfaceElementClass* element = findElbyName(elname);
	if (element)
		return element->isVisible();
	else
		return 0;
}

//SubFunction for checking selections
void InterfaceClass::CheckSelection(ID3D11DeviceContext* deviceContext, int mouseX, int mouseY)
{
	int ind;
	std::string elName;


	if (getEl(mouseX, mouseY, ind, elName)) //if element under mouse
	{
		if (ind != m_lastInd || elName != m_lastName) // if this is not element that was in last frame (to improve calc)
		{
			// updating description
			//updateElTStext(deviceContext, "Descwindow", "descwindow", 0, m_interfaceElements[ind]->getDesñ(elName));

			//if last element is still unselected - do it first. Selection of new element will be in the next frame so/
			if (!m_isUnSel)
			{
				m_interfaceElements[m_lastInd]->addUnSelCommand(m_lastName);
				m_isUnSel = 1;
			}
			else
			{
				m_lastInd = ind; //saving new params of last element
				m_lastName = elName;
				m_interfaceElements[ind]->addSelCommand(elName);
				m_isUnSel = 0;
			}
		}

		// updating position of desc
		updateElTSposX(deviceContext, "Descwindow", "descwindow", 0, mouseX);
		updateElTSposY(deviceContext, "Descwindow", "descwindow", 0, mouseY - 30);
	}
	else
	{
		// if no element under cursor - unselect last element and do nothing yet
		if (!m_isUnSel)
		{
			m_interfaceElements[m_lastInd]->addUnSelCommand(m_lastName);
			m_isUnSel = 1;
		}

		//m_interfaceElements[lastInd]->updateBMselIntens(lastName, 0);
		m_lastInd = 0;
		m_lastName = "";
		//updateElTStext("Descwindow", "descwindow", 0, 0, getElTSsize("Descwindow", "descwindow", 0), "      ");
	}

}

//Updating of Debug info
void InterfaceClass::UpdateDebug(ID3D11DeviceContext* deviceContext, D3DXVECTOR3 pos, D3DXVECTOR3 rot, int mouseX, int mouseY)
{
	bool result;
	//int buttons[95] = { 57, 242, 4042, 442, 542, 642, 842, 40, 1042, 1142, 942, 1342, 51, 12, 52, 53, 11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 3942, 39, 4251, 13, 4252, 4253, 342, 3042, 4248, 4246, 3242, 1842, 3342, 3442, 3542, 2342, 3642, 3742, 3842, 4250, 4249, 2442, 2542, 1642, 1942, 3142, 2042, 2242, 4247, 1742, 4245, 2142, 4244, 26, 43, 27, 742, 1242, 41, 30, 48, 46, 32, 18, 33, 34, 35, 23, 36, 37, 38, 50, 49, 24, 25, 16, 19, 31, 20, 22, 47, 17, 45, 21, 44, 2642, 4243, 2742, 4142 };
	int k;
	char KeyboardString[20] = "";


	//updating of debag info
	if (isElVisible("Debug_info"))
	{
		updateElTSadding(deviceContext,"Debug_info", "debug_info", 0, std::to_string(mouseX));

		updateElTSadding(deviceContext, "Debug_info", "debug_info", 1, std::to_string(mouseY));

		updateElTSadding(deviceContext, "Debug_info", "debug_info", 2, std::to_string(SystemStateManagerClass::getI().GetFps()));

		updateElTSadding(deviceContext, "Debug_info", "debug_info", 3, std::to_string(SystemStateManagerClass::getI().GetCpuPercentage()) + "%");



		updateElTSadding(deviceContext, "Debug_info", "debug_info", 6, std::to_string(pos.x));

		updateElTSadding(deviceContext, "Debug_info", "debug_info", 7, std::to_string(pos.y));

		updateElTSadding(deviceContext, "Debug_info", "debug_info", 8, std::to_string(pos.z));

		updateElTSadding(deviceContext, "Debug_info", "debug_info", 9, std::to_string(rot.x));

		updateElTSadding(deviceContext, "Debug_info", "debug_info", 10, std::to_string(rot.y));

		updateElTSadding(deviceContext, "Debug_info", "debug_info", 11, std::to_string(rot.z));

		///////////////////////////////////////////////////////

		//Change color of fps in case of value

		// If fps is 60 or above set the fps color to green.
		if (SystemStateManagerClass::getI().GetFps()>= 60)
		{
			updateElTScolor(deviceContext, "Debug_info", "debug_info", 2, D3DXVECTOR4(0, 1, 0,1));
		}

		// If fps is below 60 set the fps color to yellow.
		if (SystemStateManagerClass::getI().GetFps()< 60)
		{
			updateElTScolor(deviceContext, "Debug_info", "debug_info", 2, D3DXVECTOR4(1, 1, 0,1));
		}

		// If fps is below 30 set the fps color to red.
		if (SystemStateManagerClass::getI().GetFps()< 30)
		{
			updateElTScolor(deviceContext, "Debug_info", "debug_info", 2, D3DXVECTOR4(1, 0, 0,1));
		}

		//Peace of shit :(
		/*for (int i = 0; i < 256; i++)
			if (KeyboardState[i])
			{
				_itoa_s(i, tempString, 10);
				strcat_s(KeyboardString, tempString);
			}

		strcpy_s(tempString, " ");
		k = atoi(KeyboardString);
		for (int j = 0; j < 95; j++)

		{
			if (k == buttons[j])
			{
				tempString[0] = j + 32;
				strcpy_s(m_keyboardText, tempString);
				break;
			}
		}*/

		//updateElTStext(deviceContext, "Debug_info", "debug_info", 4, 0, getElTSsize("Debug_info", "debug_info", 4), m_resources->getGameTime()->getTimeString());
		//updateElTStext(deviceContext, "Debug_info", "debug_info", 5, 0, getElTSsize("Debug_info", "debug_info", 5), m_keyboardText);
	}


	///////////////////////////////////////////////////////////////////

	//if (isElVisible("Image_info"))
	//{
	//	//_itoa_s(mposX, tempString, 10);
	//	//updateElTSlastword("Image_info", "image_info", 0, tempString);

	//	//_itoa_s(mposY, tempString, 10);
	//	//updateElTSlastword("Image_info", "image_info", 1, tempString);

	//	_itoa_s(Color.x, tempString, 10);
	//	updateElTSadding(deviceContext, "Image_info", "image_info", 2, tempString);

	//	_itoa_s(Color.y, tempString, 10);
	//	updateElTSadding(deviceContext, "Image_info", "image_info", 3, tempString);

	//	_itoa_s(Color.z, tempString, 10);
	//	updateElTSadding(deviceContext, "Image_info", "image_info", 4, tempString);

	//	//_itoa_s(provnum, tempString, 10);
	//	//updateElTSlastword("Image_info", "image_info", 5, tempString);
	//}
}

//Actions on pick and unpick
void InterfaceClass::pick(int ind, const std::string& name)
{
	// change state of LBM and give pick command
	m_LBMDown = true;
	m_interfaceElements[m_lastInd]->addUnSelCommand(m_lastName);
	m_interfaceElements[ind]->addPickCommand(name);

	// save new params of last
	m_lastInd = ind;
	m_lastName = name;
}
void InterfaceClass::unPick(int ind, const std::string& name)
{
	//change state od LBM
	m_LBMDown = false;
	//if this the same element - return unpick command
	if (ind == m_lastInd && name == m_lastName)
		m_interfaceElements[ind]->addUnPickCommand(name);
	else
	{
		// else return unselect command
		m_interfaceElements[m_lastInd]->addUnSelCommand(m_lastName);
		endDrag(m_lastName);
	}

	m_lastInd = 0;
	m_lastName = "";
}

int InterfaceClass::getNumFromName(const std::string& name)
{
	//CalculatorClass calculator;
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
	return m_calculator->calculate(number);
}
