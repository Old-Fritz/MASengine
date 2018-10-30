#include "LoadScreenManagerClass.h"

LoadScreenManagerClass* LoadScreenManagerClass::m_instance = 0;

bool LoadScreenManagerClass::Initialize(D3DClass* D3D, ShaderManagerClass* shaders, D3DXMATRIX baseViewMatrix, HWND hwnd, PathClass* filename)
{
	bool result;
	int screenWidth = SettingsClass::getI().getIntParameter("ScreenWidth");
	int screenHeight = SettingsClass::getI().getIntParameter("ScreenHeight");

	//save graphics object
	m_D3D = D3D;
	m_shaders = shaders;
	m_baseViewMatrix = baseViewMatrix;

	m_imageElementName = PathManagerClass::getI().makePath();
	m_quoteElementName = PathManagerClass::getI().makePath();
	m_lineElementName = PathManagerClass::getI().makePath();

	result = readFromFile(filename);
	if (!result)
	{
		GM::LM()->addLog("Error 15-1");
		return false;
	}

	//initialize elements
	//init image
	m_loadImage = new(1) InterfaceElementClass;
	if (!m_loadImage)
		return false;
	result = m_loadImage->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, m_imageElementName, screenWidth, screenHeight);
	if (!result)
	{
		GM::LM()->addLog("Error 15-2");
		return false;
	}

	//init line
	m_loadLine = new(1) InterfaceElementClass;
	if (!m_loadLine)
		return false;
	result = m_loadLine->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, m_lineElementName, screenWidth, screenHeight);
	if (!result)
	{
		GM::LM()->addLog("Error 15-3");
		return false;
	}

	//init quote
	m_loadQuote = new(1) InterfaceElementClass;
	if (!m_loadQuote)
		return false;
	result = m_loadQuote->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, m_quoteElementName, screenWidth, screenHeight);
	if (!result)
	{
		GM::LM()->addLog("Error 15-4");
		return false;
	}

	//init additional
	for (int i = 0;i < m_additionalElementNames.size();i++)
	{
		InterfaceElementClass* element = new(1) InterfaceElementClass;
		if (!element)
			return false;
		result = element->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, m_additionalElementNames[i], screenWidth, screenHeight);
		if (!result)
		{
			GM::LM()->addLog("Error 15-5");
			return false;
		}
		m_additionalElements.emplace_back(element);
	}

	//make first update
	result = updateElements();
	if (!result)
	{
		return false;
	}

	return true;
}

void LoadScreenManagerClass::Shutdown()
{
	while(m_additionalElements.size())
	{
		auto element = m_additionalElements.begin();
		if (*element)
		{
			(*element)->Shutdown();
			::operator delete((*element), sizeof(*(*element)), 1);
			(*element) = 0;
		}
		m_additionalElements.erase(element);
	}

	if (m_loadQuote)
	{
		m_loadQuote->Shutdown();
		::operator delete(m_loadQuote, sizeof(*m_loadQuote), 1);
		m_loadQuote = 0;
	}
	if (m_loadLine)
	{
		m_loadLine->Shutdown();
		::operator delete(m_loadLine, sizeof(*m_loadLine), 1);
		m_loadLine = 0;
	}
	if (m_loadImage)
	{
		m_loadImage->Shutdown();
		::operator delete(m_loadImage, sizeof(*m_loadImage), 1);
		m_loadImage = 0;
	}


	if (m_instance)
	{
		::operator delete(m_instance, sizeof(*m_instance), 1);
		m_instance = 0;
	}
}

bool LoadScreenManagerClass::Render()
{
	D3DXMATRIX worldMatrix, projectionMatrix, orthoMatrix;
	bool result;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));



	// Get the world, view, projection, and ortho matrices from the camera and d3d objects.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	//render 2D
	m_D3D->TurnZBufferOff();

	//render all parts
	result = m_loadImage->Render(m_shaders->getFontShader(),m_shaders->getInterfaceShader(),m_D3D->GetDeviceContext(),worldMatrix,orthoMatrix,m_baseViewMatrix);
	if (!result)
	{
		GM::LM()->addLog("Error 15-6");
		return false;
	}
	result = m_loadLine->Render(m_shaders->getFontShader(), m_shaders->getInterfaceShader(), m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix, m_baseViewMatrix);
	if (!result)
	{
		GM::LM()->addLog("Error 15-7");
		return false;
	}
	result = m_loadQuote->Render(m_shaders->getFontShader(), m_shaders->getInterfaceShader(), m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix, m_baseViewMatrix);
	if (!result)
	{
		GM::LM()->addLog("Error 15-8");
		return false;
	}

	for (auto element = m_additionalElements.begin(); element != m_additionalElements.end(); element++)
	{
		result = (*element)->Render(m_shaders->getFontShader(), m_shaders->getInterfaceShader(), m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix, m_baseViewMatrix);
		if (!result)
		{
			GM::LM()->addLog("Error 15-9");
			return false;
		}
	}

	m_D3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

LoadScreenManagerClass & LoadScreenManagerClass::getI()
{
	if (!m_instance)
		m_instance = new(4) LoadScreenManagerClass;
	else
		return *m_instance;
}

void LoadScreenManagerClass::changeLine(const std::string & text, float perCent)
{
	bool result;

	changeLinePosition(perCent);
	result = changeLineText(text);
	if (!result)
	{
		GM::LM()->addLog("Error 15-10");
	}
	result = Render();
	if (!result)
	{
		GM::LM()->addLog("Error 15-11");
	}
	//changeTime
	result = updateElements();
	if (!result)
	{
		GM::LM()->addLog("Error 15-12");
	}
	m_lastTime = time(0);
}

LoadScreenManagerClass::LoadScreenManagerClass()
{
	m_currentQuote = -1;
	m_currentImage = -1;
	m_imageCurrentInterval = 0;
	m_quoteCurrentInterval = 0;
	m_loadImage = 0;
	m_loadLine = 0;
	m_loadQuote = 0;

	m_imageElementName = 0;
	m_quoteElementName = 0;
	m_lineElementName = 0;
}

LoadScreenManagerClass::~LoadScreenManagerClass()
{
}

bool LoadScreenManagerClass::readFromFile(PathClass*  filename)
{
	std::string temp;
	std::ifstream file;

	//open file
	file.open(filename->getPath());
	if (file.fail())
	{
		GM::LM()->addLog("Error 15-13");
		return false;
	}

	//read image
	int numOfImages;
	file >> temp >> temp >> temp; // loadImage = {
	file >> temp >> temp;
	file >> m_imageElementName; // imageElement = 
	file >> temp >> temp >> numOfImages; //numOfImages =
	file >> temp; // images:
	for (int i = 0;i < numOfImages;i++)
	{
		PathClass* path = PathManagerClass::getI().makePath();
		file >> path;
		m_loadImageNames.emplace_back(path);
	}
	file >> temp >> temp >> temp; // changeType = XXX
	if (temp == "random")
		m_imageChangeRand = 1;
	else
		m_imageChangeRand = 0;
	file >> temp >> temp >> m_imageChangeInterval; // changeInterval = XXX
	file >> temp; // }

	//read quote
	int numOfQuotes;
	file >> temp >> temp >> temp; // loadQuote   = {
	file >> temp >> temp;
	file >> m_quoteElementName; // qouteElement  = 
	file >> temp >> temp >> numOfQuotes; //numOfQuotes  =
	file >> temp; // quotes:
	for (int i = 0;i < numOfQuotes;i++)
	{
		file >> temp;
		m_loadQuoteNames.emplace_back(temp);
	}
	file >> temp >> temp >> temp; // changeType = XXX
	if (temp == "random")
		m_imageChangeRand = 1;
	else
		m_imageChangeRand = 0;
	file >> temp >> temp >> m_quoteChangeInterval; // changeInterval = XXX
	file >> temp; // }

	 //read line
	file >> temp >> temp >> temp; // loadLine    = {
	file >> temp >> temp;
	file >> m_lineElementName; // lineElement   = 
	file >> temp; // }

	//read quote
	int numOfElements;
	file >> temp >> temp >> temp; // additionalElements    = {
	file >> temp >> temp >> numOfElements; //numOfElements   =
	file >> temp; // elements:
	for (int i = 0;i < numOfElements;i++)
	{
		PathClass* path = PathManagerClass::getI().makePath();
		file >> path;
		m_additionalElementNames.emplace_back(path);
	}
	file >> temp; // }

	file.close();

	return true;
}

bool LoadScreenManagerClass::updateElements()
{
	bool result;

	//update quote element
	m_quoteCurrentInterval += time(0) - m_lastTime;
	if (m_quoteCurrentInterval >= m_quoteChangeInterval)
	{
		m_quoteCurrentInterval = 0;
		result = changeQuote();
		if (!result)
		{
			GM::LM()->addLog("Error 15-14");
			return false;
		}
	}
	//update image element
	m_imageCurrentInterval += time(0) - m_lastTime;
	if (m_imageCurrentInterval >= m_imageChangeInterval)
	{
		m_imageCurrentInterval = 0;
		result = changeImage();
		if (!result)
		{
			GM::LM()->addLog("Error 15-15");
			return false;
		}
	}

	return true;
}

void LoadScreenManagerClass::showElements()
{
	m_loadLine->setVisible(1);
	m_loadImage->setVisible(1);
	m_loadQuote->setVisible(1);
	for(int i = 0;i<m_additionalElements.size();i++)
		m_additionalElements[i]->setVisible(1);
}

void LoadScreenManagerClass::hideElements()
{
	m_loadLine->setVisible(0);
	m_loadImage->setVisible(0);
	m_loadQuote->setVisible(0);
	for (int i = 0;i<m_additionalElements.size();i++)
		m_additionalElements[i]->setVisible(0);
}

void LoadScreenManagerClass::changeLinePosition(float perCent)
{
	m_loadLine->updateBMhorPercent("line", perCent);
}

bool LoadScreenManagerClass::changeLineText(const std::string & text)
{
	bool result;
	result = m_loadLine->updateTStext(m_D3D->GetDeviceContext(), "text", 0, text);
	if (!result)
	{
		return false;
	}

	return true;
}

bool LoadScreenManagerClass::changeImage()
{
	bool result;
	int ind;

	//delete old texture
	if(m_currentImage>=0 && m_currentImage <= m_loadImageNames.size())
		TextureManagerClass::getI().deleteTexture(m_loadImageNames[m_currentImage]);

	//find ind of new image
	if (m_imageChangeRand)
	{
		m_currentImage = rand() % m_loadImageNames.size();
		ind = m_currentImage;
	}
	else
	{
		m_currentImage++;
		if (m_currentImage >= m_loadImageNames.size())
			m_currentImage = 0;
		ind = m_currentImage;
	}

	result = m_loadImage->setNewBM(m_D3D->GetDevice(), "image", m_loadImageNames[ind]);
	if (!result)
	{
		return false;
	}

	return true;
}

bool LoadScreenManagerClass::changeQuote()
{
	bool result;
	int ind;

	//find ind of new quote
	if (m_quoteChangeRand)
	{
		m_currentQuote = rand() % m_loadQuoteNames.size();
		ind = m_currentQuote;
	}
	else
	{
		m_currentQuote++;
		if (m_currentQuote >= m_loadQuoteNames.size())
			m_currentQuote = 0;
		ind = m_currentQuote;
	}

	result = m_loadQuote->updateTStext(m_D3D->GetDeviceContext(), "quote", 0, m_loadQuoteNames[ind]);
	if (!result)
	{
		return false;
	}

	return true;
}
