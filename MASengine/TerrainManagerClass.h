////////////////////////////////////////////////////////////////////////////////
// Filename: TerrainManagerClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TERRAINMANAGERCLASS_H_
#define _TERRAINMANAGERCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "TerrainClass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: TerrainManagerClass
////////////////////////////////////////////////////////////////////////////////
class TerrainManagerClass
{
public:
	TerrainManagerClass();
	TerrainManagerClass(const TerrainManagerClass&);
	~TerrainManagerClass();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string terrainFilenameBase, int numOfBlocks);
	void Shutdown();
	bool Render(TerrainShaderClass* terrainShader, ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
		D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor,
		D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower,
		float SCREEN_DEPTH, FrustumClass* frustum);

	//pick actions
	bool pick(ID3D11DeviceContext * deviceContext, D3DXVECTOR3 rayOrigin, D3DXVECTOR3 rayDirection, int& provNum, D3DXVECTOR3& point);

private:
	void translateMatrix(D3DXMATRIX& matrix, D3DXVECTOR3 transVector);

	bool initializeMapTextures(ID3D11Device* device);
private:
	std::vector<TerrainClass*> m_terrain;

	int m_mapTextureHashes[NUM_OF_MAP_TEXTURES];
};

#endif
