#include "BoxMeshClass.h"



BoxMeshClass::BoxMeshClass()
{
}

BoxMeshClass::BoxMeshClass(const BoxMeshClass &)
{
}

BoxMeshClass::~BoxMeshClass()
{
}

bool BoxMeshClass::Initialize(ID3D11Device * device, D3DXVECTOR3 minPoint, D3DXVECTOR3 maxPoint)
{
	bool result;

	m_minPoint = minPoint;
	m_maxPoint = maxPoint;

	//Init buffers
	result = InitializeBuffers(device);
	if (!result)
		return false;
	
	return true;
}

void BoxMeshClass::Shutdown()
{
	// Release the vertex and index buffers.
	ShutdownBuffers();
}

void BoxMeshClass::Render(ID3D11DeviceContext * deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}

D3DXVECTOR3 BoxMeshClass::getCenter()
{
	return m_minPoint + getSize()/2.0f;
}

D3DXVECTOR3 BoxMeshClass::getSize()
{
	return m_maxPoint - m_minPoint;
}

bool BoxMeshClass::checkFrustum(FrustumClass * frustum)
{
	return frustum->CheckPoints(m_points, m_vertexCount);
}

int BoxMeshClass::GetIndexCount()
{
	return m_indexCount;
}

bool BoxMeshClass::createVertsAndInds(unsigned long ** indices)
{
	m_vertexCount = 8;
	m_indexCount = 36;
	m_points = new(4) D3DXVECTOR3[m_vertexCount];
	*indices = new(4) unsigned long[m_indexCount];

	//create vertices
	m_points[0] = m_minPoint;
	m_points[1] = D3DXVECTOR3(m_minPoint.x,m_maxPoint.y,m_minPoint.z);
	m_points[2]= D3DXVECTOR3(m_maxPoint.x, m_maxPoint.y, m_minPoint.z);
	m_points[3] = D3DXVECTOR3(m_maxPoint.x, m_minPoint.y, m_minPoint.z);
	m_points[4] = D3DXVECTOR3(m_minPoint.x, m_minPoint.y, m_maxPoint.z);
	m_points[5] = D3DXVECTOR3(m_minPoint.x, m_maxPoint.y, m_maxPoint.z);
	m_points[6] = m_maxPoint;
	m_points[7] = D3DXVECTOR3(m_maxPoint.x, m_minPoint.y, m_maxPoint.z);

	//fill other types of vertex
	/*for (int i = 0;i < m_vertexCount;i++)
	{
		(*vertices)[i].normal = D3DXVECTOR3(0, 0, 0);
		(*vertices)[i].texture = D3DXVECTOR2(0, 0);
	}*/

	unsigned long arr[36] = { 0,1,2,   2,3,0,   0,4,5,   5,1,0,   4,7,6,   6,5,4,   2,6,7,   7,3,2,   3,7,4,   4,0,3,   1,5,6,   6,2,1 };
	//create indicies
	for(int i = 0;i<m_indexCount;i++)
		(*indices)[i] = arr[i];

	return true;
}

bool BoxMeshClass::InitializeBuffers(ID3D11Device * device)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	unsigned long* indices;

	//create verticies and indices
	result = createVertsAndInds(&indices);
	if (!result)
	{
		return false;
	}

	// Set up the description of the vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(D3DXVECTOR3)* m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = m_points;

	// Now finally create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void BoxMeshClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

void BoxMeshClass::RenderBuffers(ID3D11DeviceContext * deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(D3DXVECTOR3);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}
