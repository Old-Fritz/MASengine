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
#include "MeshLoaderClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: HeightMapLoaderClass
////////////////////////////////////////////////////////////////////////////////

class HeightMapLoaderClass : public MeshLoaderClass
{
public:
	HeightMapLoaderClass();
	HeightMapLoaderClass(const HeightMapLoaderClass&);
	~HeightMapLoaderClass();

	bool loadModel(PathClass* filename, float sizeX = 0, float sizeY = 0, float sizeZ = 0);
	bool createVertsAndInds(void** vertices, unsigned long** indices);
	 void calcVertAndIndCount(int& vertexCount, int& indexCount);

	void Shutdown();
private:
	//Creating height map
	bool loadHeightMap(PathClass* filename);
	void normalizeHeightMap(float sizeX = 0, float sizeY = 0, float sizeZ = 0);
	bool calculateNormals();
	void calculateTextureCoordinates();
private:
	int m_terrainWidth;
	int m_terrainHeight;
};

#endif