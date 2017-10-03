#include "meshclass.h"

MeshClass::MeshClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_model = 0;
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

int MeshClass::GetIndexCount()
{
	return m_indexCount;
}

void MeshClass::getBox(float & xSize, float & ySize, float & zSize)
{
	xSize = ySize = zSize = 1;
}

bool MeshClass::createVertsAndInds(VertexType ** vertices, unsigned long ** indices)
{
	// Create the vertex array.
	*vertices = new(3) VertexType[m_vertexCount];
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
		(*vertices)[i].position = D3DXVECTOR3(m_model[i].x, m_model[i].y, m_model[i].z);
		(*vertices)[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
		(*vertices)[i].normal = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		(*indices)[i] = i;
	}
}

bool MeshClass::InitializeBuffers(ID3D11Device * device, VertexType* vertices, unsigned long * indices)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;


	// Set up the description of the vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
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

bool MeshClass::LoadModel(PathClass* filename)
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
