////////////////////////////////////////////////////////////////////////////////
// Filename: meshClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MESHCLASS_H_
#define _MESHCLASS_H_


//////////////
// INCLUDES //
//////////////
#include "HeightMapLoaderClass.h"
#include "BoxMeshClass.h"


///////////////////////
// MY CLASS INCLUDES //
///////////////////////



////////////////////////////////////////////////////////////////////////////////
// Class name: MeshClass
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

	bool Initialize(ID3D11Device* device, PathClass* filename, int sizeX = 0, int sizeY = 0, int sizeZ = 0);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);
	void RenderBoxMesh(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();
private:
	bool InitializeBuffers(ID3D11Device* device, VertexType* vertices, unsigned long* indices);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	bool LoadModel(PathClass* filename);

	bool buildBoxMesh(ID3D11Device* device, D3DXVECTOR3 minPoint, D3DXVECTOR3 maxPoint);
	void findExtrPoints(VertexType* vertices, D3DXVECTOR3& minPoint, D3DXVECTOR3& maxPoint);
private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	ModelType* m_model;

	BoxMeshClass* m_boxMesh;
	bool m_isBoxMeshRendering = 0;
};

#endif