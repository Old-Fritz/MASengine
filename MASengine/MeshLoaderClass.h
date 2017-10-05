////////////////////////////////////////////////////////////////////////////////
// Filename: MeshLoaderClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _MESHLOADERCLASS_H_
#define _MESHLOADERCLASS_H_

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
// Class name: MeshLoaderClass
////////////////////////////////////////////////////////////////////////////////

class MeshLoaderClass
{
protected:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};
	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};
public:
	MeshLoaderClass();
	MeshLoaderClass(const MeshLoaderClass&);
	~MeshLoaderClass();

	virtual bool loadModel(PathClass* filename, float sizeX = 1, float sizeY = 1, float sizeZ = 1);
	virtual bool createVertsAndInds(void** vertices, unsigned long** indices);
	virtual void calcVertAndIndCount(int& vertexCount, int& indexCount);

	virtual void Shutdown();
private:
	bool loadMesh(PathClass* filename);
	bool normalizeMesh(float scaleX, float scaleY, float scaleZ);
protected:
	ModelType* m_model;
	int m_vertexCount;
	int m_indexCount;
};

#endif