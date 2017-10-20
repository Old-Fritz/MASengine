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
	m_provTextureHash = m_provFilename->getHash();

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

	return true;
}


void TerrainClass::Shutdown()
{
}

bool TerrainClass::Render(TerrainShaderClass * terrainShader, ID3D11DeviceContext * deviceContext, D3DXMATRIX worldMatrix,
	D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, D3DXVECTOR3 lightDirection,
	D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower,
	float SCREEN_DEPTH, FrustumClass* frustum)
{
	bool result;

	//find best mesh to render
	int lvl = getLvlByDist(Utils::calcDist(m_position.x, m_position.y, m_position.z, cameraPosition.x, cameraPosition.y, cameraPosition.z));
	MeshClass* mesh = MeshManagerClass::getI().getModel(m_meshHash[lvl]);

	//check if current mesh in frustum
	if (!mesh->checkFrustum(frustum,m_position))
		return true;

	// Render mesh
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

	//add new block region
	BlockRegionClass* region = new(4) BlockRegionClass;
	if (!region)
		return false;
	result = region->Initialize(&file, m_ID);
	if (!result)
		return false;
	ProvRegionManagerClass::getI().addProvRegion(GlobalManagerClass::BLOCK, region);

	return true;
}

D3DXVECTOR4 * TerrainClass::getProvColor()
{
	//get region
	ProvRegionClass* region = ProvRegionManagerClass::getI().getProvRegion(GlobalManagerClass::BLOCK, m_ID);

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
	ProvRegionClass* region = ProvRegionManagerClass::getI().getProvRegion(GlobalManagerClass::BLOCK, m_ID);

	auto provs = region->getProvs();

	if (provs->size() > index)
	{
		//get prov by index
		return (*provs)[index]->getID();
	}
	else
		return 0;
}
