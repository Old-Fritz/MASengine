#include "TerrainManagerClass.h"

TerrainManagerClass::TerrainManagerClass()
{
	m_skyTexture = 0;
	m_sky = 0;
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

		m_terrain.emplace_back(terrain);

		if(i%100==0)
			LoadScreenManagerClass::getI().changeLine("block" +std::to_string(i)+ "_init", 0.3f + 0.25f*(i / numOfBlocks));
	}

	result = initializeMapTextures(device);
	if (!result)
		return false;

	//init base commands names
	m_pickCommand = "terrainPick";
	m_unPickCommand = "terrainUnPick";

	// create water translation counter
	SystemStateManagerClass::getI().getTimer()->addCounter("waterTranslation", 0.00001f, 1.0f, 0);

	// init render textures
	m_skyTexture = new(1) RenderTextureClass;
	if (!m_skyTexture)
		return false;

	result = m_skyTexture->Initialize(device, (float)SettingsClass::getI().getIntParameter("ScreenHeight"), (float)SettingsClass::getI().getIntParameter("ScreenHeight"));
	if (!result)
		return false;

	

	// create sky
	m_sky = new(1) SkyModelClass;
	if (!m_sky)
	{
		return false;
	}
	result = m_sky->Initialize(device, deviceContext, SettingsClass::getI().getPathParameter("SkyDescFilename"));
	if (!result)
		return false;

	// Set render textures to all blocks
	for (int i = 0;i < m_terrain.size();i++)
	{
		m_terrain[i]->setRenderTextures(m_skyTexture);
	}

	return true;
}

void TerrainManagerClass::Shutdown()
{
	if (m_sky)
	{
		m_sky->Shutdown();
		::operator delete(m_sky, sizeof(*m_sky), 1);
		m_sky = 0;
	}
	if (m_skyTexture)
	{
		m_skyTexture->Shutdown();
		::operator delete(m_skyTexture, sizeof(*m_skyTexture), 1);
		m_skyTexture = 0;
	}

	for (int i = m_terrain.size()-1;i >= 0;i--)
	{
		m_terrain[i]->Shutdown();
		::operator delete(m_terrain[i], sizeof(*m_terrain[i]), 1);
		m_terrain[i] = 0;
	}
	m_terrain.clear();
}

bool TerrainManagerClass::Render(D3DClass* D3D, TerrainShaderClass * terrainShader, WaterShaderClass* waterShader, FillShaderClass* fillShader, SkyShaderClass* skyShader,
	D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, std::vector<D3DXMATRIX> topViewMatrix, std::vector<LightClass::PointLightType*> lights,
	D3DXVECTOR3 cameraPosition, float SCREEN_DEPTH, FrustumClass * frustum)
{
	bool result;
	ID3D11ShaderResourceView** mapTextures;
	float waterHeight, waterTranslation;

	//get map textures
	mapTextures = TextureManagerClass::getI().getTexturesArray(m_mapTextureHashes, NUM_OF_MAP_TEXTURES);

	//set water params
	waterHeight = SettingsClass::getI().getFloatParameter("WaterHeight");
	waterTranslation = SystemStateManagerClass::getI().getTimer()->getCounter("waterTranslation");

	//render sky to texture
	m_skyTexture->SetRenderTarget(D3D->GetDeviceContext(), D3D->GetTextureDepthStencilView());
	m_skyTexture->ClearRenderTarget(D3D->GetDeviceContext(), D3D->GetTextureDepthStencilView(), D3DXVECTOR4(1, 0, 0, 1));

	// Render mesh
	D3DXMATRIX skyMatrix = topViewMatrix[0];
	MeshClass::translateMatrix(skyMatrix, D3DXVECTOR3(0,0, -waterHeight));
	result = m_sky->Render(D3D->GetDeviceContext(), skyShader, worldMatrix, skyMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}
	// return to original target

	D3D->SetBackBufferRenderTarget();

	//render all blocks
	
	
	for (auto block = m_terrain.begin();block != m_terrain.end();block++)
	{
		//render block
		result = (*block)->Render(D3D, terrainShader, waterShader, fillShader, skyShader, worldMatrix, viewMatrix, projectionMatrix, topViewMatrix, mapTextures, lights,
			cameraPosition, SCREEN_DEPTH, waterHeight, waterTranslation, frustum);
		if (!result)
			return false;

	}


	//render sky
	//result = m_sky->Render(D3D->GetDeviceContext(), skyShader, worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
		return false;

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

std::vector<D3DXVECTOR3> TerrainManagerClass::getBlockTopCameraPos()
{

	std::vector<D3DXVECTOR3> output;
	float terrainHeight = m_terrain[0]->getTerrainHeight() / 2;
	float terrainWidth = m_terrain[0]->getTerrainWidth() / 2;
	float shift = (float)SettingsClass::getI().getIntParameter("ScreenWidth") / (float)SettingsClass::getI().getIntParameter("ScreenHeight");

	//normilize position
	for (int i = 0;i < 10;i++)
	{
		D3DXVECTOR3 pos;
		pos.x = terrainHeight*shift*pow(2, i);
		pos.y = (terrainHeight / tan(SettingsClass::getI().getFloatParameter("FieldOfView") / 2.0f)* pow(2, i) + SettingsClass::getI().getFloatParameter("WaterHeight"));
		pos.z = terrainWidth - terrainWidth * (pow(2, i) - 1);
		output.emplace_back(pos);
	}
	
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
