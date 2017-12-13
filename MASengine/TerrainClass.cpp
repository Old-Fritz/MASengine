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

	m_id = id;
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
	m_provTextureHash = m_provFilename->getHash();

	result = TextureManagerClass::getI().addTexture(device, m_physFilename);
	if (!result)
	{
		return false;
	}
	m_physTextureHash = m_physFilename->getHash();

	result = TextureManagerClass::getI().addTexture(device, m_waterFilename);
	if (!result)
	{
		return false;
	}
	m_waterTextureHash = m_waterFilename->getHash();

	result = TextureManagerClass::getI().addTexture(device, m_waterNormalFilename);
	if (!result)
	{
		return false;
	}
	m_waterNormalTexureHash = m_waterNormalFilename->getHash();

	//add meshes
	for (int i = NUM_OF_LVLS - 1; i >= 0; i--)
	{
		//create new PathClass
		m_hmapFilenames[i] = PathManagerClass::getI().makePath(m_hmapFilenameBase + "LVL" + std::to_string(i+1) + ".bmp");
		//create hash
		m_meshHash[i] = m_hmapFilenames[i]->getHash();
		//load model
		result = MeshManagerClass::getI().addModel(device, m_hmapFilenames[i], m_terrainWidth,SettingsClass::getI().getFloatParameter("HeightMapModifier"), m_terrainHeight); 
		if (!result)
		{
			return false;
		}
	}

	result = MeshManagerClass::getI().addModel(device, m_waterMeshFilename,1,1,1);
	if (!result)
	{
		return false;
	}
	m_waterHash = m_waterMeshFilename->getHash();

	//create render to textures

	m_fillTexture.second = new(1) RenderTextureClass;
	if (!m_fillTexture.second)
		return false;

	result = m_fillTexture.second->Initialize(device, (float)SettingsClass::getI().getIntParameter("ScreenHeight") / pow(2, MAX_WATER_FILL_LEVEL),
		(float)SettingsClass::getI().getIntParameter("ScreenHeight") / pow(2, MAX_WATER_FILL_LEVEL));
	if (!result)
		return false;

	m_fillTexture.first = -1;

	return true;
}


void TerrainClass::Shutdown()
{
	
	if (m_fillTexture.second)
	{
		m_fillTexture.second->Shutdown();
		::operator delete(m_fillTexture.second, sizeof(*m_fillTexture.second), 1);
		m_fillTexture.second = 0;
	}
	
	
}

void TerrainClass::setRenderTextures(RenderTextureClass* skyTexture)
{
	m_skyTexture = skyTexture;
}

bool TerrainClass::Render(D3DClass* D3D, TerrainShaderClass* terrainShader, WaterShaderClass* waterShader, FillShaderClass* fillShader, SkyShaderClass* skyShader, D3DXMATRIX worldMatrix,
	D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, std::vector<D3DXMATRIX> topViewMatrix, ID3D11ShaderResourceView** mapTextures,
	std::vector<LightClass::PointLightType*> lights, D3DXVECTOR3 cameraPosition,
	float SCREEN_DEPTH, float waterHeight, float waterTranslation, FrustumClass* frustum)
{
	bool result;

	//find best mesh to render
	int lvl = getLvlByDist(Utils::calcDist(m_position.x, m_position.y, m_position.z, cameraPosition.x, cameraPosition.y, cameraPosition.z));
	MeshClass* mesh = MeshManagerClass::getI().getModel(m_meshHash[lvl]);

	//check if current mesh in frustum
	if (!mesh->checkFrustum(frustum,m_position))
		return true;

	// render something in texture
	//MeshClass::translateMatrix(reflectionMatrix, D3DXVECTOR3(m_position.x, 0, m_position.z));
	result = renderToTexture(D3D, fillShader, skyShader, worldMatrix, viewMatrix, projectionMatrix, topViewMatrix,  mesh, waterHeight,lvl);
	if (!result)
	{
		return false;
	}

	//translate world Matrix first
	MeshClass::translateMatrix(worldMatrix, m_position);

	// Render mesh
	mesh->Render(D3D->GetDeviceContext());
	result = terrainShader->Render(D3D->GetDeviceContext(), mesh->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		TextureManagerClass::getI().getTexture(m_provTextureHash), TextureManagerClass::getI().getTexture(m_physTextureHash), m_skyTexture->GetShaderResourceView(),
		mapTextures, lights, cameraPosition, getProvColor(), waterHeight);
	if (!result)
	{
		return false;
	}
	
	// Render water
	result = renderWater(waterShader, D3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
		lights, cameraPosition, waterHeight, waterTranslation,lvl);
	if (!result)
	{
		return false;
	}
	
	return true;
}

bool TerrainClass::renderWater(WaterShaderClass* waterShader, ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
	D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, std::vector<LightClass::PointLightType*> lights, D3DXVECTOR3 cameraPosition,
	float waterHeight, float waterTranslation, int lvl)
{
	bool result;

	MeshClass* mesh = MeshManagerClass::getI().getModel(m_waterHash);

	MeshClass::translateMatrix(worldMatrix,D3DXVECTOR3(0, waterHeight,0));

	mesh->Render(deviceContext);
	result = waterShader->Render(deviceContext, mesh->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		TextureManagerClass::getI().getTexture(m_waterNormalTexureHash), TextureManagerClass::getI().getTexture(m_waterTextureHash),
		m_fillTexture.second->GetShaderResourceView(), m_skyTexture->GetShaderResourceView(), lights, cameraPosition, waterTranslation);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TerrainClass::renderToTexture(D3DClass * D3D, FillShaderClass * fillShader, SkyShaderClass* skyShader, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,
	std::vector<D3DXMATRIX> topViewMatrix, MeshClass* terrainMesh, float waterHeight, int lvl)
{
	bool result;

	
	//prepare render to texture
	//check texture for rendering
	if (m_fillTexture.first!=lvl)
	{
		m_fillTexture.second->SetRenderTarget(D3D->GetDeviceContext(), D3D->GetTextureDepthStencilView());
		m_fillTexture.second->ClearRenderTarget(D3D->GetDeviceContext(), D3D->GetTextureDepthStencilView(), D3DXVECTOR4(1, 0, 0, 1));
		// Render mesh
		terrainMesh->Render(D3D->GetDeviceContext());
		result = fillShader->Render(D3D->GetDeviceContext(), terrainMesh->GetIndexCount(), worldMatrix, topViewMatrix[MAX_WATER_FILL_LEVEL], projectionMatrix, D3DXVECTOR4(0, 1, 0, -waterHeight));
		if (!result)
		{
			return false;
		}
		D3D->SetBackBufferRenderTarget();
		m_fillTexture.first = lvl;
	}

	return true;
}

bool  TerrainClass::pick(ID3D11DeviceContext * deviceContext, D3DXVECTOR3 rayOrigin, D3DXVECTOR3 rayDirection, int& provNum, D3DXVECTOR3& point)
{
	bool result;

	//normilize ray origin with position of block
	rayOrigin -= m_position;
	D3DXVECTOR3 color = { 0, 0, 0 };

	//check the best mesh
	result = MeshManagerClass::getI().getModel(m_meshHash[0])->intersect(deviceContext, rayOrigin, rayDirection, point);
	if (!result)
		return false;
	
	//get color of pixel in bmp
	result = getColorFromBMP(point.x, point.z, color, PathManagerClass::getI().makePath(m_hmapFilenameBase + ".bmp"));
	if (!result)
		return false;

	//get provnum
	provNum = getProvNum(color.z);

	return true;

}

bool TerrainClass::getColorFromBMP(float x, float y, D3DXVECTOR3 & color, PathClass* filename)
{
	FILE* filePtr;
	int error;
	unsigned int count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	int terrainWidth, terrainHeight;

	// Open the height map file in binary.
	error = fopen_s(&filePtr, filename->getPath().c_str(), "rb");
	if (error != 0)
	{
		return false;
	}

	// Read in the file header.
	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Read in the bitmap info header.
	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Save the dimensions of the terrain.
	terrainWidth = bitmapInfoHeader.biWidth;
	terrainHeight = bitmapInfoHeader.biHeight;

	//get coords in bmp
	int bX, bY;
	bX = (x / m_terrainWidth) * terrainWidth;
	bY = (y / m_terrainHeight) * terrainHeight;
	//bX = terrainWidth - bX;
	//bY = terrainHeight - bY;
	//y = (float)y / 128.0f * (float)terrainWidth - 1;
	//x = (float)x / 128.0f * (float)terrainHeight - 1;
	//if (x < 0)
	//	x = 0;
	//if (y < 0)
	//	y = 0;

	// Move to the beginning of the bitmap data of current pixel.
	fseek(filePtr, bitmapFileHeader.bfOffBits + (terrainWidth * 3)*bY + bX * 3, SEEK_SET);

	// Read data in color
	color.z = abs(getc(filePtr));
	color.y = abs(getc(filePtr));
	color.x = abs(getc(filePtr));

	// Close the file.
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	return true;
}

D3DXVECTOR3 TerrainClass::getPosition()
{
	return m_position;
}

float TerrainClass::getTerrainHeight()
{
	return m_terrainHeight;
}

float TerrainClass::getTerrainWidth()
{
	return m_terrainWidth;
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
	file >> temp >> temp;
	m_physFilename = PathManagerClass::getI().makePath();
	file >> m_physFilename;
	//water
	file >> temp >> temp;
	m_waterMeshFilename = PathManagerClass::getI().makePath();
	file >> m_waterMeshFilename;
	file >> temp >> temp;
	m_waterFilename = PathManagerClass::getI().makePath();
	file >> m_waterFilename;
	file >> temp >> temp;
	m_waterNormalFilename = PathManagerClass::getI().makePath();
	file >> m_waterNormalFilename;


	//don`t initialize region if this is base block (0)
	if (!m_id)
		return true;

	//add new block region
	BlockRegionClass* region = new(4) BlockRegionClass;
	if (!region)
		return false;
	result = region->Initialize(&file, m_id);
	if (!result)
		return false;
	ProvRegionManagerClass::getI().addProvRegion(GlobalManagerClass::BLOCK, region);

	return true;
}

D3DXVECTOR4 * TerrainClass::getProvColor()
{
	//get region
	ProvRegionClass* region = ProvRegionManagerClass::getI().getProvRegion(GlobalManagerClass::BLOCK, m_id);

	auto provs = region->getProvs();
	D3DXVECTOR4* provColors = new(2) D3DXVECTOR4[256];
	
	int i = 0;
	for (auto prov = provs->begin();prov!= provs->end() && i<256;i++, prov++)
	{
		provColors[i] = (*prov)->getLayers()->getMainColor();
	}
	//fill last colors with white
	while (i < 256)
	{
		provColors[i] = D3DXVECTOR4(1, 1, 1, 1);
		i++;
	}

	//some fun tests
	/*for (int i = 0;i < 256;i++)
	{
		provColors[i] = D3DXVECTOR4((float)rand()/ (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX);
	}*/

	return provColors;
}

int TerrainClass::getLvlByDist(float dist)
{
	int lvl;
	if (dist > 1600)
		lvl = 3;
	else if (dist > 1000)
		lvl = 2;
	else if (dist > 500)
		lvl = 1;
	else
		lvl = 0;

	return lvl;

}

int TerrainClass::getProvNum(int index)
{
	//get region
	ProvRegionClass* region = ProvRegionManagerClass::getI().getProvRegion(GlobalManagerClass::BLOCK, m_id);

	auto provs = region->getProvs();

	if (provs->size() > index)
	{
		//get prov by index
		return (*provs)[index]->getID();
	}
	else
		return 0;
}
