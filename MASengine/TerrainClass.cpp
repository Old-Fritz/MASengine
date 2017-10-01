#include "TerrainClass.h"

TerrainClass::TerrainClass()
{
	m_provs = 0;
}

TerrainClass::TerrainClass(const TerrainClass &)
{
}

TerrainClass::~TerrainClass()
{
}

bool TerrainClass::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, std::string blockFilename)
{
	bool result;

	//read info from file
	result = readFromFile(ModManagerClass::getI().getDirectory(blockFilename) + blockFilename);
	if (!result)
	{
		return false;
	}

	//add textures
	result = TextureManagerClass::getI().addTexture(device, m_provFilename);
	if (!result)
	{
		return false;
	}

	//add meshes
	for (int i = NUM_OF_LVLS - 1; i >= 0; i--)
	{
		result = MeshManagerClass::getI().addModel(device, m_hmapfilename + std::to_string(i) + ".bmp",i);
		if (!result)
		{
			return false;
		}
	}

	return true;
}

void TerrainClass::Shutdown()
{
}

bool TerrainClass::Render(TerrainShaderClass * terrainShader, ID3D11DeviceContext * deviceContext, D3DXMATRIX worldMatrix,
	D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, D3DXVECTOR3 lightDirection,
	D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower, float SCREEN_DEPTH, int lvl)
{
	bool result;

	if (lvl > NUM_OF_LVLS - 1 || lvl < 0)
		lvl = 0;

	MeshClass* mesh = MeshManagerClass::getI().getModel(m_meshHash[lvl]);

	result = terrainShader->Render(deviceContext, mesh->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, TextureManagerClass::getI().getTexture(m_provTextureHash), lightDirection, ambientColor,
		diffuseColor, cameraPosition, specularColor, specularPower, getProvColor());
	if (!result)
	{
		return false;
	}
	
	return true;
}

D3DXVECTOR3 TerrainClass::pick(int x, int y)
{
	return D3DXVECTOR3();
}

D3DXVECTOR3 TerrainClass::GetPosition()
{
	return D3DXVECTOR3();
}

bool TerrainClass::readFromFile(std::string filename)
{
	return false;
}

D3DXVECTOR4 * TerrainClass::getProvColor()
{
	return nullptr;
}
