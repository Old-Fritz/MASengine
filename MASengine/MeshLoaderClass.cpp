#include "MeshLoaderClass.h"

MeshLoaderClass::MeshLoaderClass()
{
}

MeshLoaderClass::MeshLoaderClass(const MeshLoaderClass &)
{
}

MeshLoaderClass::~MeshLoaderClass()
{
}

bool MeshLoaderClass::createBuffers(ID3D11Device * device, ID3D11Buffer** vertexBuffer, ID3D11Buffer** indexBuffer)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	VertexType* vertices;
	unsigned long * indices;

	// Get data
	result = createVertsAndInds(&vertices, &indices);
	if (!result)
		return false;

	findExtrPoints(vertices);

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
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, vertexBuffer);
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
	result = device->CreateBuffer(&indexBufferDesc, &indexData, indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool MeshLoaderClass::loadModel(PathClass * filename, float sizeX, float sizeY, float sizeZ)
{
	bool result;

	// Load in the mesh
	result = loadMesh(filename);
	if (!result)
	{
		return false;
	}

	// Normalize the scale of mesh
	normalizeMesh(sizeX, sizeY, sizeZ);

	return true;
}

void MeshLoaderClass::getExtrPoints(D3DXVECTOR3 & minPoint, D3DXVECTOR3 & maxPoint)
{
	minPoint = m_minPoint;
	maxPoint = m_maxPoint;
}

void MeshLoaderClass::findExtrPoints(VertexType* vertices)
{
	//create default values
	m_minPoint = m_maxPoint = vertices[0].position;
	for (int i = 0;i < m_vertexCount;i++)
	{
		//find minimum
		if (vertices[i].position.x < m_minPoint.x)
			m_minPoint.x = vertices[i].position.x;
		if (vertices[i].position.y < m_minPoint.y)
			m_minPoint.y = vertices[i].position.y;
		if (vertices[i].position.z < m_minPoint.z)
			m_minPoint.z = vertices[i].position.z;

		//find maximum
		if (vertices[i].position.x > m_maxPoint.x)
			m_maxPoint.x = vertices[i].position.x;
		if (vertices[i].position.y > m_maxPoint.y)
			m_maxPoint.y = vertices[i].position.y;
		if (vertices[i].position.z > m_maxPoint.z)
			m_maxPoint.z = vertices[i].position.z;
	}
}

bool MeshLoaderClass::createVertsAndInds(VertexType ** vertices, unsigned long ** indices)
{
	// Create the vertex array.
	(*(VertexType**)vertices) = new(3) VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	*indices = new(3) unsigned long[m_vertexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for (int i = 0; i<m_vertexCount; i++)
	{
		(*(VertexType**)vertices)[i].position = D3DXVECTOR3(m_model[i].x, m_model[i].y, m_model[i].z);
		(*(VertexType**)vertices)[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
		(*(VertexType**)vertices)[i].normal = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		(*indices)[i] = i;
	}

	return true;
}

void MeshLoaderClass::calcVertAndIndCount(int & vertexCount, int & indexCount)
{
	vertexCount = m_vertexCount;
	indexCount = m_indexCount;
}

void MeshLoaderClass::Shutdown()
{
}

bool MeshLoaderClass::loadMesh(PathClass* filename)
{
	bool result;


	std::ifstream fin;
	char input;
	int i;


	// Open the model file.  If it could not open the file then exit.
	fin.open(filename->getPath());
	if (fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new(3) ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for (i = 0; i<m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	// Close the model file.
	fin.close();

	return true;
}

void MeshLoaderClass::normalizeMesh(float scaleX, float scaleY, float scaleZ)
{
	for (int i = 0;i < m_vertexCount;i++)
	{
		m_model[i].x *= scaleX;
		m_model[i].y *= scaleY;
		m_model[i].z *= scaleZ;
	}
}
