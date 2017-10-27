////////////////////////////////////////////////////////////////////////////////
// Filename: terrainclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TERRAINCLASS_H_
#define _TERRAINCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "ProvManagerClass.h"
#include "GlobalManagerClass.h"
#include "ProvRegionManagerClass.h"
#include "TerrainShaderClass.h"

//////////////
// GLOBALS //
/////////////
const int NUM_OF_LVLS = 4;


////////////////////////////////////////////////////////////////////////////////
// Class name: TerrainClass
////////////////////////////////////////////////////////////////////////////////
class TerrainClass
{
public:
	TerrainClass();
	TerrainClass(const TerrainClass&);
	~TerrainClass();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, PathClass* blockFilename, int id);
	void Shutdown();
	bool Render(TerrainShaderClass* terrainShader, ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
		D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView** mapTextures, D3DXVECTOR3 lightDirection,
		D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower,
		float SCREEN_DEPTH, FrustumClass* frustum);

	//pick actions
	bool pick(ID3D11DeviceContext * deviceContext, D3DXVECTOR3 rayOrigin,D3DXVECTOR3 rayDirection, int& provNum, D3DXVECTOR3& point);

	bool getColorFromBMP(float x, float y, D3DXVECTOR3& color, PathClass* filename);

	//Getters
	D3DXVECTOR3 getPosition();

	
private:
	//block info
	bool readFromFile(PathClass* filename);
	D3DXVECTOR4* getProvColor();
	int getLvlByDist(float dist);
	int getProvNum(int index);
private:
	int m_id;

	float m_terrainWidth, m_terrainHeight;
	int m_meshHash[NUM_OF_LVLS];
	PathClass* m_hmapFilenames[NUM_OF_LVLS];
	std::string m_hmapFilenameBase;

	//textures
	int m_provTextureHash;
	PathClass* m_provFilename;

	int m_physTextureHash;
	PathClass* m_physFilename;

	//info from file
	D3DXVECTOR3 m_position; //position on map
};

#endif
