#include "TerrainManagerClass.h"

TerrainManagerClass::TerrainManagerClass()
{
	m_fillTexture = 0;
}
TerrainManagerClass::TerrainManagerClass(const TerrainManagerClass &)
{
}
TerrainManagerClass::~TerrainManagerClass()
{
}

bool TerrainManagerClass::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, std::string terrainFilenameBase, int numOfBlocks)
{
	bool result;

	//create and init all blocks (+1 base(0))
	for (int i = 0;i < numOfBlocks + 1;i++)
	{
		TerrainClass* terrain = new(1) TerrainClass;
		if (!terrain)
			return false;

		result = terrain->Initialize(device, deviceContext, PathManagerClass::getI().makePath(terrainFilenameBase + std::to_string(i) + ".txt"), i);
		if (!result)
			return false;

		result = initializeMapTextures(device);
		if (!result)
			return false;

		m_terrain.emplace_back(terrain);
	}

	//init base commands names
	m_pickCommand = "terrainPick";
	m_unPickCommand = "terrainUnPick";

	// create water translation counter
	SystemStateManagerClass::getI().getTimer()->addCounter("waterTranslation", 0.00001f, 1.0f, 0);

	// init render textures
	m_fillTexture = new(1) RenderTextureClass;
	if (!m_fillTexture)
		return false;

	//result = m_fillTexture->Initialize(device, m_terrain[0]->getTerrainWidth()*5, m_terrain[0]->getTerrainHeight()*5);
	result = m_fillTexture->Initialize(device, (float)SettingsClass::getI().getIntParameter("ScreenHeight") / pow(2, 3), (float)SettingsClass::getI().getIntParameter("ScreenHeight") / pow(2, 3));
	if (!result)
		return false;

	// Set render textures to all blocks
	for (int i = 0;i < m_terrain.size();i++)
	{
		m_terrain[i]->setRenderTextures(m_fillTexture);
	}

	return true;
}

void TerrainManagerClass::Shutdown()
{
	if (m_fillTexture)
	{
		m_fillTexture->Shutdown();
		::operator delete(m_fillTexture, sizeof(*m_fillTexture), 1);
		m_fillTexture = 0;
	}
	for (int i = m_terrain.size()-1;i >= 0;i--)
	{
		m_terrain[i]->Shutdown();
		::operator delete(m_terrain[i], sizeof(*m_terrain[i]), 1);
		m_terrain[i] = 0;
	}
	m_terrain.clear();
}

bool TerrainManagerClass::Render(D3DClass* D3D, TerrainShaderClass * terrainShader, WaterShaderClass* waterShader, FillShaderClass* fillShader, D3DXMATRIX worldMatrix,
	D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, D3DXMATRIX topViewMatrix, D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor,
	D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower,
	float SCREEN_DEPTH, FrustumClass * frustum)
{
	bool result;
	ID3D11ShaderResourceView** mapTextures;
	float waterHeight, waterTranslation;

	//get map textures
	mapTextures = TextureManagerClass::getI().getTexturesArray(m_mapTextureHashes, NUM_OF_MAP_TEXTURES);

	//set water params
	waterHeight = SettingsClass::getI().getFloatParameter("WaterHeight");
	waterTranslation = SystemStateManagerClass::getI().getTimer()->getCounter("waterTranslation");

	//render all blocks
	for (auto block = m_terrain.begin();block != m_terrain.end();block++)
	{
		//render block
		result = (*block)->Render(D3D, terrainShader, waterShader, fillShader, worldMatrix, viewMatrix, projectionMatrix, topViewMatrix, mapTextures, lightDirection,
			ambientColor, diffuseColor, cameraPosition, specularColor, specularPower, SCREEN_DEPTH, waterHeight, waterTranslation, frustum);
		if (!result)
			return false;
	}

	return true;
}

bool TerrainManagerClass::pick(ID3D11DeviceContext * deviceContext, D3DXVECTOR3 rayOrigin, D3DXVECTOR3 rayDirection,
	int & provNum, D3DXVECTOR3 & point)
{
	bool result;

	//pick all blocks
	for (auto block = m_terrain.begin();block != m_terrain.end();block++)
	{
		//pick block
		result = (*block)->pick(deviceContext,rayOrigin,rayDirection,provNum,point);
		if (result)
			return true;
	}

	return false;
}

std::string TerrainManagerClass::getPickCommandName()
{
	return m_pickCommand;
}

std::string TerrainManagerClass::getUnPickCommandName()
{
	return m_unPickCommand;
}

D3DXVECTOR3 TerrainManagerClass::getBlockTopCameraPos()
{
	int lvl = 3;

	D3DXVECTOR3 output;
	float terrainHeight = m_terrain[0]->getTerrainHeight() / 2;
	float terrainWidth = m_terrain[0]->getTerrainWidth() / 2;
	float shift = (float)SettingsClass::getI().getIntParameter("ScreenWidth") / (float)SettingsClass::getI().getIntParameter("ScreenHeight");
	//normilize position
	output.x = terrainHeight*shift*pow(2, lvl);
	output.y = (terrainHeight / tan(SettingsClass::getI().getFloatParameter("FieldOfView") / 2.0f)* pow(2, lvl) + SettingsClass::getI().getFloatParameter("WaterHeight")) ;
	output.z = terrainWidth - terrainWidth * (pow(2, lvl)-1);
	return output;
}



bool TerrainManagerClass::initializeMapTextures(ID3D11Device * device)
{
	bool result;
	//init all map textures
	for (int i = 0;i < NUM_OF_MAP_TEXTURES;i++)
	{
		PathClass* path = PathManagerClass::getI().makePath(SettingsClass::getI().getStrParameter("MapTextureFilenameBase") + std::to_string(i) + ".dds");

		result = TextureManagerClass::getI().addTexture(device,path);
		if (!result)
			return false;

		m_mapTextureHashes[i] = path->getHash();
	}
	return true;
}
