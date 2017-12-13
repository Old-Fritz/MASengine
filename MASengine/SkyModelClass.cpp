#include "SkyModelClass.h"

SkyModelClass::SkyModelClass()
{
}
SkyModelClass::SkyModelClass(const SkyModelClass &)
{
}
SkyModelClass::~SkyModelClass()
{
}

bool SkyModelClass::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, PathClass * skyFilename)
{
	bool result;

	//read info from file
	result = readFromFile(skyFilename);
	if (!result)
	{
		return false;
	}

	//add textures
	result = TextureManagerClass::getI().addTexture(device, m_texture1Filename);
	if (!result)
	{
		return false;
	}
	m_texture1Hash = m_texture1Filename->getHash();

	result = TextureManagerClass::getI().addTexture(device, m_texture2Filename);
	if (!result)
	{
		return false;
	}
	m_texture2Hash = m_texture2Filename->getHash();

	// add mesh
	result = MeshManagerClass::getI().addModel(device, m_meshFilename, m_skyWidth, (m_skyWidth+ m_skyHeight)/2.0f, m_skyHeight);
	if (!result)
	{
		return false;
	}
	m_meshHash = m_meshFilename->getHash();

	// create counters
	SystemStateManagerClass::getI().getTimer()->addCounter("firstSkyTranslationX",0.000005f,1,0);
	SystemStateManagerClass::getI().getTimer()->addCounter("firstSkyTranslationY", 0.00001f, 1, 0);
	SystemStateManagerClass::getI().getTimer()->addCounter("secondSkyTranslationX", 0.000001f, 1, 0);
	SystemStateManagerClass::getI().getTimer()->addCounter("secondSkyTranslationY", 0.000002f, 1, 0);

	return true;
}

void SkyModelClass::Shutdown()
{
}

bool SkyModelClass::Render(ID3D11DeviceContext * deviceContext, SkyShaderClass * skyShader, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	bool result;
	D3DXVECTOR2 firstTranslation;
	D3DXVECTOR2 secondTranslation;

	//translate world Matrix first
	MeshClass::translateMatrix(worldMatrix, m_position);
	//get mesh
	MeshClass* mesh = MeshManagerClass::getI().getModel(m_meshHash);

	//Get translations
	firstTranslation.x = SystemStateManagerClass::getI().getTimer()->getCounter("firstSkyTranslationX");
	firstTranslation.y = SystemStateManagerClass::getI().getTimer()->getCounter("firstSkyTranslationX");
	secondTranslation.x = SystemStateManagerClass::getI().getTimer()->getCounter("secondSkyTranslationX");
	secondTranslation.y = SystemStateManagerClass::getI().getTimer()->getCounter("secondSkyTranslationY");

	// render sky
	mesh->Render(deviceContext);
	result = skyShader->Render(deviceContext, mesh->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, TextureManagerClass::getI().getTexture(m_texture1Hash),
		TextureManagerClass::getI().getTexture(m_texture2Hash), firstTranslation, secondTranslation, 1);
	if (!result)
		return false;

	return true;
}

bool SkyModelClass::readFromFile(PathClass * filename)
{
	bool result;

	std::ifstream file;
	file.open(filename->getPath());
	if (!file.is_open())
	{
		return false;
	}

	std::string temp;
	//read position
	file >> temp >> temp >> m_position.x;
	file >> temp >> temp >> m_position.y;
	file >> temp >> temp >> m_position.z;

	//read size of sky
	file >> temp >> temp >> m_skyWidth;
	file >> temp >> temp >> m_skyHeight;

	//read filepathes
	file >> temp >> temp;
	m_texture1Filename = PathManagerClass::getI().makePath();
	file >> m_texture1Filename;
	file >> temp >> temp;
	m_texture2Filename = PathManagerClass::getI().makePath();
	file >> m_texture2Filename;

	file >> temp >> temp;
	m_meshFilename = PathManagerClass::getI().makePath();
	file >> m_meshFilename;


	return true;
}
