#include "LoadScreenManagerClass.h"

LoadScreenManagerClass* LoadScreenManagerClass::m_instance = 0;

bool LoadScreenManagerClass::Initialize(D3DClass* D3D, ShaderManagerClass* shaders, D3DXMATRIX baseViewMatrix, HWND hwnd, const std::string & filename)
{
	bool result;
	int screenWidth = SettingsClass::getI().getIntParameter("screenWidth");
	int screenHeight = SettingsClass::getI().getIntParameter("screenHeight");;

	//save graphics object
	m_D3D = D3D;
	m_shaders = shaders;
	m_baseViewMatrix = baseViewMatrix;

	result = readFromFile(filename);
	if (!result)
	{
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
		return false;
	}

	//init line
	m_loadLine = new(1) InterfaceElementClass;
	if (!m_loadLine)
		return false;
	result = m_loadLine->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, m_lineElementName, screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}

	//init quote
	m_loadQuote = new(1) InterfaceElementClass;
	if (!m_loadQuote)
		return false;
	result = m_loadQuote->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, m_quoteElementName, screenWidth, screenHeight);
	if (!result)
	{
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
			return false;
		}
		m_additionalElements.emplace_back(element);
	}

	return true;
}

void LoadScreenManagerClass::Shutdown()
{
	for (int i = 0;i < m_additionalElementNames.size();i++)
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
		return false;
	}
	result = m_loadLine->Render(m_shaders->getFontShader(), m_shaders->getInterfaceShader(), m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix, m_baseViewMatrix);
	if (!result)
	{
		return false;
	}
	result = m_loadQuote->Render(m_shaders->getFontShader(), m_shaders->getInterfaceShader(), m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix, m_baseViewMatrix);
	if (!result)
	{
		return false;
	}

	for (auto element = m_additionalElements.begin(); element != m_additionalElements.end(); element++)
	{
		result = (*element)->Render(m_shaders->getFontShader(), m_shaders->getInterfaceShader(), m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix, m_baseViewMatrix);
		if (!result)
		{
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

	}
	result = Render();
	if (!result)
	{

	}
	//changeTime
	result = updateElements();
	if (!result)
	{

	}
	m_lastTime = time(0);
}

LoadScreenManagerClass::LoadScreenManagerClass()
{
	m_currentQuote = 0;
	m_currentImage = 0;
	m_imageCurrentInterval = 0;
	m_quoteCurrentInterval = 0;
}

LoadScreenManagerClass::~LoadScreenManagerClass()
{
}

bool LoadScreenManagerClass::readFromFile(const std::string & filename)
{
	std::string temp;
	std::ifstream file;

	//open file
	file.open(filename);
	if (file.fail())
	{
		return false;
	}

	//read image
	int numOfImages;
	file >> temp >> temp >> temp; // loadImage = {
	file >> temp >> temp >> m_imageElementName; // imageElement = 
	file >> temp >> temp >> numOfImages; //numOfImages =
	file >> temp; // images:
	for (int i = 0;i < numOfImages;i++)
	{
		file >> temp;
		m_loadImageNames.emplace_back(temp);
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
	file >> temp >> temp >> m_quoteElementName; // qouteElement  = 
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
	file >> temp >> temp >> m_lineElementName; // lineElement   = 
	file >> temp; // }

	//read quote
	int numOfElements;
	file >> temp >> temp >> temp; // additionalElements    = {
	file >> temp >> temp >> numOfElements; //numOfElements   =
	file >> temp; // elements:
	for (int i = 0;i < numOfElements;i++)
	{
		file >> temp;
		m_additionalElementNames.emplace_back(temp);
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
			return false;
		}
	}

	return true;
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
}

bool LoadScreenManagerClass::changeImage()
{
	bool result;
	int ind;

	//delete old texture
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
