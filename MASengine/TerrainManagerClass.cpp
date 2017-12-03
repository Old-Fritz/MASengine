#include "TerrainManagerClass.h"

TerrainManagerClass::TerrainManagerClass()
{
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

	return true;
}

void TerrainManagerClass::Shutdown()
{
	for (int i = m_terrain.size()-1;i >= 0;i--)
	{
		m_terrain[i]->Shutdown();
		::operator delete(m_terrain[i], sizeof(*m_terrain[i]), 1);
		m_terrain[i] = 0;
	}
	m_terrain.clear();
}

bool TerrainManagerClass::Render(TerrainShaderClass * terrainShader, WaterShaderClass* waterShader, ID3D11DeviceContext * deviceContext, D3DXMATRIX worldMatrix,
	D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor,
	D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower,
	float SCREEN_DEPTH, FrustumClass * frustum)
{
	bool result;
	ID3D11ShaderResourceView** mapTextures;

	//get map textures
	mapTextures = TextureManagerClass::getI().getTexturesArray(m_mapTextureHashes, NUM_OF_MAP_TEXTURES);

	//render all blocks
	for (auto block = m_terrain.begin();block != m_terrain.end();block++)
	{
		//translate world Matrix first
		MeshClass::translateMatrix(worldMatrix, (*block)->getPosition());

		//render block
		result = (*block)->Render(terrainShader, waterShader, deviceContext, worldMatrix, viewMatrix, projectionMatrix, mapTextures, lightDirection,
			ambientColor, diffuseColor, cameraPosition, specularColor, specularPower, SCREEN_DEPTH, frustum);
		if (!result)
			return false;

		//translate world matrix back
		MeshClass::translateMatrix(worldMatrix, -(*block)->getPosition());
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
