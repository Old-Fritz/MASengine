////////////////////////////////////////////////////////////////////////////////
// Filename: meshClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MESHCLASS_H_
#define _MESHCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <fstream>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include <d3d10.h>
#include <d3dx10.h>
#include "MemoryManagerClass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class MeshClass
{
private:
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
	MeshClass();
	MeshClass(const MeshClass&);
	~MeshClass();

	bool Initialize(ID3D10Device* device, const std::string& filename);
	void Shutdown();
	void Render(ID3D10Device* device);

	int GetIndexCount();
	void getBox(float& xSize, float& ySize, float& zSize);
private:
	bool InitializeBuffers(ID3D10Device* device);
	void ShutdownBuffers();
	void RenderBuffers(ID3D10Device* device);

	bool LoadModel(const std::string& filename);
	void ReleaseModel();

private:
	ID3D10Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	ModelType* m_model;
};

#endif