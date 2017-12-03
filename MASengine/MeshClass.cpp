#include "meshclass.h"

MeshClass::MeshClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_isBoxMeshRendering = 0;
}
MeshClass::MeshClass(const MeshClass &)
{
}
MeshClass::~MeshClass()
{
}

bool MeshClass::Initialize(ID3D11Device * device, PathClass* filename, int sizeX, int sizeY, int sizeZ)
{
	bool result;

	VertexType* verticies;
	unsigned long* indicies;
	D3DXVECTOR3 minPoint, maxPoint;

	//create loader
	MeshLoaderClass* loader;
	if (Utils::getHash(filename->getExpansion()) == Utils::getHash("bmp"))
		loader = new(3) HeightMapLoaderClass;
	else
		loader = new(3) MeshLoaderClass;

	// Load in the model data.
	result = loader->loadModel(filename,sizeX,sizeY,sizeZ);
	if (!result)
	{
		return false;
	}

	//get vertex and index count
	loader->calcVertAndIndCount(m_vertexCount, m_indexCount);

	result = loader->createVertsAndInds((void**)&verticies, &indicies);
	if (!result)
	{
		return false;
	}

	// find points of extremum to create box mesh
	findExtrPoints(verticies, minPoint, maxPoint);

	//create box
	result = buildBoxMesh(device, minPoint, maxPoint);
	if (!result)
		return false;

	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	result = InitializeBuffers(device, verticies, indicies);
	if (!result)
	{
		return false;
	}


	MemoryManagerClass::getI().cleanTemp();

	return true;
}

void MeshClass::Shutdown()
{

	// Release the vertex and index buffers.
	ShutdownBuffers();
}

void MeshClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}

void MeshClass::RenderBoxMesh(ID3D11DeviceContext * deviceContext)
{
	m_boxMesh->Render(deviceContext);
	m_isBoxMeshRendering = 1;
}

int MeshClass::GetIndexCount()
{
	if (!m_isBoxMeshRendering)
		return m_indexCount;
	//get info about box if it was rendered
	else
	{
		return m_boxMesh->GetIndexCount();
		//reverse to normal state
		m_isBoxMeshRendering = 0;
	}
	
}

bool MeshClass::InitializeBuffers(ID3D11Device * device, VertexType* vertices, unsigned long * indices)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;


	// Set up the description of the vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_STAGING;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* m_vertexCount;
	//vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.BindFlags = 0;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;

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

void MeshClass::ShutdownBuffers()
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

	return;
}

void MeshClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}


bool MeshClass::buildBoxMesh(ID3D11Device* device, D3DXVECTOR3 minPoint, D3DXVECTOR3 maxPoint)
{
	bool result;

	m_boxMesh = new(4) BoxMeshClass;
	if (!m_boxMesh)
		return false;

	result = m_boxMesh->Initialize(device, minPoint, maxPoint);
	if (!result)
		return false;

	return true;
}

void MeshClass::findExtrPoints(VertexType* vertices, D3DXVECTOR3 & minPoint, D3DXVECTOR3 & maxPoint)
{
	//create default values
	minPoint = maxPoint = vertices[0].position;
	for (int i = 0;i < m_vertexCount;i++)
	{
		//find minimum
		if (vertices[i].position.x < minPoint.x)
			minPoint.x = vertices[i].position.x;
		if (vertices[i].position.y < minPoint.y)
			minPoint.y = vertices[i].position.y;
		if (vertices[i].position.z < minPoint.z)
			minPoint.z = vertices[i].position.z;

		//find maximum
		if (vertices[i].position.x > maxPoint.x)
			maxPoint.x = vertices[i].position.x;
		if (vertices[i].position.y > maxPoint.y)
			maxPoint.y = vertices[i].position.y;
		if (vertices[i].position.z > maxPoint.z)
			maxPoint.z = vertices[i].position.z;
	}
}

bool MeshClass::getVertsAndInds(ID3D11DeviceContext* deviceContext, VertexType** verticies, unsigned long** indices)
{
	D3D11_MAPPED_SUBRESOURCE verticesPtr;
	D3D11_MAPPED_SUBRESOURCE indicesPtr;
	HRESULT result;

	// Lock the vertex buffer.
	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_READ, 0, &verticesPtr);
	if (FAILED(result))
	{
		return false;
	}

	*verticies = (VertexType*)verticesPtr.pData;

	// Lock the index buffer.
	result = deviceContext->Map(m_indexBuffer, 0, D3D11_MAP_READ, 0, &indicesPtr);
	if (FAILED(result))
	{
		return false;
	}

	*indices = (unsigned long*)indicesPtr.pData;

	//Unlock buffers
	deviceContext->Unmap(m_vertexBuffer, 0);
	deviceContext->Unmap(m_indexBuffer, 0);

	return true;
}

void MeshClass::translateMatrix(D3DXMATRIX & matrix, D3DXVECTOR3 transVector)
{
	D3DXMATRIX translateMatrix;

	D3DXMatrixTranslation(&translateMatrix, transVector.x, transVector.y, transVector.z);
	D3DXMatrixMultiply(&matrix, &matrix, &translateMatrix);
}

void MeshClass::rotateMatrix(D3DXMATRIX & matrix, D3DXVECTOR3 rotVector)
{
	D3DXMATRIX rotateMatrix;

	D3DXMatrixRotationYawPitchRoll(&rotateMatrix, rotVector.x, rotVector.y, rotVector.z);
	D3DXMatrixMultiply(&matrix, &matrix, &rotateMatrix);
}

bool MeshClass::intersect(ID3D11DeviceContext * deviceContext, D3DXVECTOR3 rayOrigin, D3DXVECTOR3 rayDirection, D3DXVECTOR3 & point)
{
	bool result;
	VertexType* verticies;
	unsigned long* indicies;
	D3DXVECTOR3 res;

	point = D3DXVECTOR3(0, 0, 0);

	//first check box mesh
	result = m_boxMesh->intersect(rayOrigin,rayDirection);
	if (!result)
		return false;

	//get verticies and indicies from buffers
	result = getVertsAndInds(deviceContext, &verticies, &indicies);
	if (!result)
		return false;

	//check all triangles
	for (int i = 0; i < m_indexCount / 3;i++)
	{
		result = m_boxMesh->triangleHitTest(rayOrigin, rayDirection, verticies[indicies[i * 3]].position, verticies[indicies[i * 3 + 1]].position, verticies[indicies[i * 3 + 2]].position, res);
		if (result)
		{
			//calculate point and return true
			point = rayOrigin + rayDirection * res.x;
			return true;
		}
	}

	return false;

}

bool MeshClass::checkFrustum(FrustumClass * frustum, D3DXVECTOR3 position)
{
	return m_boxMesh->checkFrustum(frustum,position);
}