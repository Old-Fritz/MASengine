#include "TerrainClass.h"

TerrainClass::TerrainClass()
{
}
TerrainClass::TerrainClass(const TerrainClass &)
{
}
TerrainClass::~TerrainClass()
{
}

bool TerrainClass::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, PathClass* blockFilename, int id)
{
	bool result;

	m_ID = id;
	//read info from file
	result = readFromFile(blockFilename);
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
		//create new PathClass
		m_hmapFilenames[i] = PathManagerClass::getI().makePath(m_hmapFilenameBase + "LVL" + std::to_string(i+1) + ".bmp");
		//create hash
		m_meshHash[i] = m_hmapFilenames[i]->getHash();
		//load model
		result = MeshManagerClass::getI().addModel(device, m_hmapFilenames[i], m_terrainWidth,m_terrainHeight); 
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
	
	mesh->Render(deviceContext);
	result = terrainShader->Render(deviceContext, mesh->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		TextureManagerClass::getI().getTexture(m_provTextureHash), lightDirection, ambientColor,
		diffuseColor, cameraPosition, specularColor, specularPower, getProvColor());
	if (!result)
	{
		return false;
	}
	
	return true;
}

D3DXVECTOR3 TerrainClass::pick(int x, int y)
{
	return D3DXVECTOR3(1,1,1);
}

D3DXVECTOR3 TerrainClass::GetPosition()
{
	return m_position;
}

bool TerrainClass::readFromFile(PathClass* filename)
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

	//read size of block
	file >> temp >> temp >> m_terrainWidth;
	file >> temp >> temp >> m_terrainHeight;

	//read filepathes
	file >> temp >> temp >> m_hmapFilenameBase;
	file >> temp >> temp;
	m_provFilename = PathManagerClass::getI().makePath();
	file >> m_provFilename;

	return true;
}

D3DXVECTOR4 * TerrainClass::getProvColor()
{
	//get region
	ProvRegionClass* provs = ProvRegionManagerClass::getI().getProvRegion(m_ID);

	auto provsIDs = provs->getProvIDs();
	D3DXVECTOR4* provColors = new(2) D3DXVECTOR4[256];
	
	int i = 0;
	for (auto ID = provsIDs->begin();ID!=provsIDs->end() || i<256;i++,ID++)
	{
		provColors[i] = ProvManagerClass::getI().getProv(*ID)->getLayers()->getMainColor();
	}
	//fill last colors with white
	while (i < 256)
	{
		provColors[i] = D3DXVECTOR4(1, 1, 1, 1);
	}

	return provColors;
}