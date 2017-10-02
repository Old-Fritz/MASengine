////////////////////////////////////////////////////////////////////////////////
// Filename: HeightMapLoaderClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _HEIGHTMAPLOADERCLASS_H_
#define _HEIGHTMAPLOADERCLASS_H_

//////////////
// INCLUDES //
//////////////


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "PathManagerClass.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10math.h>
#include "MemoryManagerClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: MeshManagerClass
////////////////////////////////////////////////////////////////////////////////

class HeightMapLoaderClass
{
public:
	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};
public:
	HeightMapLoaderClass();
	HeightMapLoaderClass(const HeightMapLoaderClass&);
	~HeightMapLoaderClass();

	bool loadHeightMap(ID3D11Device* device, PathClass* filename, float width, float height, void** model,
		int& vertexCount, int& indexCount);
	void Shutdown();
private:
	//Creating height map
	bool loadModel(PathClass* filename);
	void normalizeHeightMap(float width, float height);
	bool calculateNormals();
	void calculateTextureCoordinates();
private:
	ModelType* m_heightMap;
	int m_terrainWidth;
	int m_terrainHeight;
};

#endif