#include "BoxMeshClass.h"

unsigned long BOX_MESH_INDICIES[36] = { 0,1,2,   2,3,0,   0,4,5,   5,1,0,   4,7,6,   6,5,4,   2,6,7,   7,3,2,   3,7,4,   4,0,3,   1,5,6,   6,2,1 };

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
	*indices = new(3) unsigned long[m_indexCount];

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

	//create indicies
	for(int i = 0;i<m_indexCount;i++)
		(*indices)[i] = BOX_MESH_INDICIES[i];

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
	vertexBufferDesc.Usage = D3D11_USAGE_STAGING;
	vertexBufferDesc.ByteWidth = sizeof(D3DXVECTOR3)* m_vertexCount;
	//vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.BindFlags = 0;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
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
	indexBufferDesc.Usage = D3D11_USAGE_STAGING;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* m_indexCount;
	//indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.BindFlags = 0;
	indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
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



bool BoxMeshClass::intersect( D3DXVECTOR3 rayOrigin, D3DXVECTOR3 rayDirection)
{
	bool result;
	D3DXVECTOR3 res;

	//check all triangles
	for (int i = 0; i < m_indexCount/3;i++)
	{
		result = triangleHitTest(rayOrigin, rayDirection, m_points[BOX_MESH_INDICIES[i * 3]], m_points[BOX_MESH_INDICIES[i * 3 + 1]], m_points[BOX_MESH_INDICIES[i * 3 + 2]], res);
		if (result)
			return true;
	}

	return false;
}

bool BoxMeshClass::triangleHitTest(D3DXVECTOR3 rayOrigin, D3DXVECTOR3 rayDirection, D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2, D3DXVECTOR3 & res)
{
	//transporant all points so v0 in (0,0,0)
	D3DXVECTOR3 E1 = v1 - v0;
	D3DXVECTOR3 E2 = v2 - v0;
	D3DXVECTOR3 T = rayOrigin - v0;
	//calc some help vectors
	D3DXVECTOR3 P, Q;
	D3DXVec3Cross(&P, &rayDirection, &E2);
	D3DXVec3Cross(&Q, &T, &E1);

	//calc t u v by formul
	res = (1.0f / D3DXVec3Dot(&P, &E1)) * D3DXVECTOR3(D3DXVec3Dot(&Q, &E2), D3DXVec3Dot(&P, &T), D3DXVec3Dot(&Q, &rayDirection));

	
	//compare 2 formuls of point 
	//D3DXVECTOR3 delta = ((rayOrigin + res.x*rayDirection) - ((1.0f - res.y - res.z)*v0 + res.y * v1 + res.z*v2));

	//check if u and v are correct
	if (res.y>=0.0f && res.y<=1.0f && res.z>=0.0f && res.z<=1.0f && res.y+res.z <=1.0f)
		return true;

	return false;
}
