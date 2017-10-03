////////////////////////////////////////////////////////////////////////////////
// Filename: meshClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MESHCLASS_H_
#define _MESHCLASS_H_


//////////////
// INCLUDES //
//////////////
#include "HeightMapLoaderClass.h"


///////////////////////
// MY CLASS INCLUDES //
///////////////////////



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

	bool Initialize(ID3D11Device* device, PathClass* filename);
	bool Initialize(ID3D11Device* device, PathClass* filename, int width, int height);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();
	void getBox(float& xSize, float& ySize, float& zSize);
private:
	bool createVertsAndInds(VertexType** vertices, unsigned long** indices);
	bool InitializeBuffers(ID3D11Device* device, VertexType* vertices, unsigned long* indices);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	bool LoadModel(PathClass* filename);

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	ModelType* m_model;
};

#endif