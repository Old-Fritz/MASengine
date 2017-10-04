////////////////////////////////////////////////////////////////////////////////
// Filename: BoxMeshClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _BOXMESHCLASS_H_
#define _BOXMESHCLASS_H_


//////////////
// INCLUDES //
//////////////
#include "PathManagerClass.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10math.h>
#include "MemoryManagerClass.h"


///////////////////////
// MY CLASS INCLUDES //
///////////////////////



////////////////////////////////////////////////////////////////////////////////
// Class name: BoxMeshClass
////////////////////////////////////////////////////////////////////////////////
class BoxMeshClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};
public:
	BoxMeshClass();
	BoxMeshClass(const BoxMeshClass&);
	~BoxMeshClass();

	bool Initialize(ID3D11Device* device, D3DXVECTOR3 minPoint, D3DXVECTOR3 maxPoint);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();
private:
	bool createVertsAndInds(D3DXVECTOR3** vertices, unsigned long** indices);
	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	D3DXVECTOR3 m_minPoint;
	D3DXVECTOR3 m_maxPoint;
};

#endif