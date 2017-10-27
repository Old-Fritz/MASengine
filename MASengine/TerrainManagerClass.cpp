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

		m_terrain.emplace_back(terrain);
	}

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

bool TerrainManagerClass::Render(TerrainShaderClass * terrainShader, ID3D11DeviceContext * deviceContext, D3DXMATRIX worldMatrix,
	D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor,
	D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower,
	float SCREEN_DEPTH, FrustumClass * frustum)
{
	bool result;

	//render all blocks
	for (auto block = m_terrain.begin();block != m_terrain.end();block++)
	{
		//translate world Matrix first
		translateMatrix(worldMatrix, (*block)->getPosition());

		//render block
		result = (*block)->Render(terrainShader, deviceContext, worldMatrix, viewMatrix, projectionMatrix, lightDirection,
			ambientColor, diffuseColor, cameraPosition, specularColor, specularPower, SCREEN_DEPTH, frustum);
		if (!result)
			return false;

		//translate world matrix back
		translateMatrix(worldMatrix, -(*block)->getPosition());
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

void TerrainManagerClass::translateMatrix(D3DXMATRIX& matrix, D3DXVECTOR3 transVector)
{
	D3DXMATRIX translateMatrix;

	D3DXMatrixTranslation(&translateMatrix, transVector.x, transVector.y, transVector.z);
	D3DXMatrixMultiply(&matrix, &matrix, &translateMatrix);
}
