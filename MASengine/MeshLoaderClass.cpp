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

bool MeshLoaderClass::createVertsAndInds(void ** vertices, unsigned long ** indices)
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
