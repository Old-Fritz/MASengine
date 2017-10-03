#include "HeightMapLoaderClass.h"

HeightMapLoaderClass::HeightMapLoaderClass()
{
}
HeightMapLoaderClass::HeightMapLoaderClass(const HeightMapLoaderClass &)
{
}
HeightMapLoaderClass::~HeightMapLoaderClass()
{
}

bool HeightMapLoaderClass::loadHeightMap(ID3D11Device * device, PathClass * filename, float width, float height, void** model,
	int& vertexCount, int& indexCount)
{
	bool result;

	// Load in the height map
	result = loadModel(filename);
	if (!result)
	{
		return false;
	}
	
	// Normalize the height of the height map.
	normalizeHeightMap(width,height);

	// Calculate the texture coordinates.
	calculateTextureCoordinates();

	// Calculate the normals for the terrain data.
	result = calculateNormals();
	if (!result)
	{
		return false;
	}

	*model = m_heightMap;
	vertexCount = (m_terrainWidth - 1) * (m_terrainHeight - 1) * 6;
	indexCount = vertexCount;
	return true;
}

bool HeightMapLoaderClass::createVertsAndInds(void ** vertices, unsigned long ** indices)
{
	
	int index;
	int index1, index2, index3, index4;
	float tu, tv;


	// Calculate the number of vertices in the terrain mesh.
	int vertexCount = (m_terrainWidth - 1) * (m_terrainHeight - 1) * 6;
	// Set the index count to the same as the vertex count.
	int indexCount = vertexCount;

	// Create the vertex array.
	(*(VertexType**)vertices) = new(3) VertexType[vertexCount];
	if (!(*(VertexType**)vertices))
	{
		return false;
	}

	// Create the index array.
	(*indices) = new unsigned long[indexCount];
	if (!(*indices))
	{
		return false;
	}

	// Initialize the index to the vertex buffer.
	index = 0;

	// Load the vertex and index array with the terrain data.
	for (int j = 0; j<(m_terrainHeight - 1); j++)
	{
		for (int i = 0; i<(m_terrainWidth - 1); i++)
		{
			index1 = (m_terrainHeight * j) + i;          // Bottom left.
			index2 = (m_terrainHeight * j) + (i + 1);      // Bottom right.
			index3 = (m_terrainHeight * (j + 1)) + i;      // Upper left.
			index4 = (m_terrainHeight * (j + 1)) + (i + 1);  // Upper right.

															 // Upper left.
			tv = m_heightMap[index3].tv;

			// Modify the texture coordinates to cover the top edge.
			//if (tv == 1.0f) { tv = 0.0f; }

			(*(VertexType**)vertices)[index].position = D3DXVECTOR3(m_heightMap[index3].x, m_heightMap[index3].y, m_heightMap[index3].z);
			(*(VertexType**)vertices)[index].texture = D3DXVECTOR2(m_heightMap[index3].tu, tv);
			(*(VertexType**)vertices)[index].normal = D3DXVECTOR3(m_heightMap[index3].nx, m_heightMap[index3].ny, m_heightMap[index3].nz);
			(*indices)[index] = index;
			index++;

			if (abs((*(VertexType**)vertices)[index - 1].position.y) < 13)
				index = index;
			// Upper right.
			tu = m_heightMap[index4].tu;
			tv = m_heightMap[index4].tv;

			// Modify the texture coordinates to cover the top and right edge.
			//if (tu == 0.0f) { tu = 1.0f; }
			//if (tv == 1.0f) { tv = 0.0f; }

			(*(VertexType**)vertices)[index].position = D3DXVECTOR3(m_heightMap[index4].x, m_heightMap[index4].y, m_heightMap[index4].z);
			(*(VertexType**)vertices)[index].texture = D3DXVECTOR2(tu, tv);
			(*(VertexType**)vertices)[index].normal = D3DXVECTOR3(m_heightMap[index4].nx, m_heightMap[index4].ny, m_heightMap[index4].nz);
			(*indices)[index] = index;
			index++;

			if (abs((*(VertexType**)vertices)[index - 1].position.y) < 13)
				index = index;
			// Bottom left.
			(*(VertexType**)vertices)[index].position = D3DXVECTOR3(m_heightMap[index1].x, m_heightMap[index1].y, m_heightMap[index1].z);
			(*(VertexType**)vertices)[index].texture = D3DXVECTOR2(m_heightMap[index1].tu, m_heightMap[index1].tv);
			(*(VertexType**)vertices)[index].normal = D3DXVECTOR3(m_heightMap[index1].nx, m_heightMap[index1].ny, m_heightMap[index1].nz);
			(*indices)[index] = index;
			index++;

			if (abs((*(VertexType**)vertices)[index - 1].position.y) < 13)
				index = index;

			// Bottom left.
			(*(VertexType**)vertices)[index].position = D3DXVECTOR3(m_heightMap[index1].x, m_heightMap[index1].y, m_heightMap[index1].z);
			(*(VertexType**)vertices)[index].texture = D3DXVECTOR2(m_heightMap[index1].tu, m_heightMap[index1].tv);
			(*(VertexType**)vertices)[index].normal = D3DXVECTOR3(m_heightMap[index1].nx, m_heightMap[index1].ny, m_heightMap[index1].nz);
			(*indices)[index] = index;
			index++;

			if (abs((*(VertexType**)vertices)[index - 1].position.y) < 13)
				index = index;

			// Upper right.
			tu = m_heightMap[index4].tu;
			tv = m_heightMap[index4].tv;

			// Modify the texture coordinates to cover the top and right edge.
			//if (tu == 0.0f) { tu = 1.0f; }
			//if (tv == 1.0f) { tv = 0.0f; }

			(*(VertexType**)vertices)[index].position = D3DXVECTOR3(m_heightMap[index4].x, m_heightMap[index4].y, m_heightMap[index4].z);
			(*(VertexType**)vertices)[index].texture = D3DXVECTOR2(tu, tv);
			(*(VertexType**)vertices)[index].normal = D3DXVECTOR3(m_heightMap[index4].nx, m_heightMap[index4].ny, m_heightMap[index4].nz);
			(*indices)[index] = index;
			index++;

			if (abs((*(VertexType**)vertices)[index - 1].position.y) < 13)
				index = index;

			// Bottom right.
			tu = m_heightMap[index2].tu;

			// Modify the texture coordinates to cover the right edge.
			//if (tu == 0.0f) { tu = 1.0f; }

			(*(VertexType**)vertices)[index].position = D3DXVECTOR3(m_heightMap[index2].x, m_heightMap[index2].y, m_heightMap[index2].z);
			(*(VertexType**)vertices)[index].texture = D3DXVECTOR2(tu, m_heightMap[index2].tv);
			(*(VertexType**)vertices)[index].normal = D3DXVECTOR3(m_heightMap[index2].nx, m_heightMap[index2].ny, m_heightMap[index2].nz);
			(*indices)[index] = index;
			index++;
			if (abs((*(VertexType**)vertices)[index - 1].position.y) < 13)
				index = index;
		}
	}
}

void HeightMapLoaderClass::Shutdown()
{
}

bool HeightMapLoaderClass::loadModel(PathClass * filename)
{
	FILE* filePtr;
	int error;
	unsigned int count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	int imageSize, i, j, k, index;
	unsigned char* bitmapImage;
	unsigned char height;

	// Open the height map file in binary.
	error = fopen_s(&filePtr, filename->getPath().c_str(), "rb");
	if (error != 0)
	{
		return false;
	}

	// Read in the file header.
	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Read in the bitmap info header.
	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Save the dimensions of the terrain.
	m_terrainWidth = bitmapInfoHeader.biWidth;
	m_terrainHeight = bitmapInfoHeader.biHeight;

	// Calculate the size of the bitmap image data.
	imageSize = m_terrainWidth * m_terrainHeight * 3;

	// Allocate memory for the bitmap image data.
	bitmapImage = new(3) unsigned char[imageSize];
	if (!bitmapImage)
	{
		return false;
	}

	// Move to the beginning of the bitmap data.
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);


	// Create the structure to hold the height map data.
	m_heightMap = new(3) ModelType[m_terrainWidth * m_terrainHeight];
	if (!m_heightMap)
	{
		return false;
	}

	// Initialize the position in the image data buffer.
	k = 0;
	int d = 0;
	// Read the image data into the height map.
	for (j = 0; j<m_terrainHeight; j++)
	{
		for (i = 0; i<m_terrainWidth; i++)
		{
			height = getc(filePtr);

			index = (m_terrainWidth * j) + i;

			m_heightMap[index].x = (float)i;
			m_heightMap[index].y = (float)height;
			m_heightMap[index].z = (float)j;

			//d++;

			getc(filePtr);
			getc(filePtr);
		}
		getc(filePtr);
	}

	// Close the file.
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	return true;
}

void HeightMapLoaderClass::normalizeHeightMap(float width, float height)
{
	float widthMultiplier = width / ((float)m_terrainWidth - 1);
	float heightMultiplier = height / ((float)m_terrainHeight - 1);


	//normilize middle blocks
	for (int i = 1; i<m_terrainHeight; i++)
	{
		for (int j = 1; j<m_terrainWidth; j++)
		{
			m_heightMap[(m_terrainWidth * i) + j].x *= widthMultiplier;
			m_heightMap[(m_terrainWidth * i) + j].z *= heightMultiplier;
		}
	}
	//normilize left blocks
	for(int i = 1;i<m_terrainHeight-1;i++)
		m_heightMap[(m_terrainWidth * i)].z *= heightMultiplier;
	//normilize bottom blocks
	for (int i = 1;i<m_terrainWidth-1;i++)
		m_heightMap[i].x *= widthMultiplier;
	//normilize right blocks
	for (int i = 0;i < m_terrainHeight;i++)
		m_heightMap[(m_terrainWidth * i) + m_terrainWidth-1].x = m_heightMap[(m_terrainWidth * i)].x + width;
	//normilize up blocks
	for (int i = 0;i < m_terrainWidth;i++)
		m_heightMap[(m_terrainWidth * (m_terrainHeight-1)) + i].z = m_heightMap[i].z + height;

	//normilize height
	for (int i = 0; i<m_terrainHeight; i++)
	{
		for (int j = 0; j<m_terrainWidth; j++)
		{
			m_heightMap[(m_terrainWidth * i) + j].y /= -15.0f;
		}
	}
}

bool HeightMapLoaderClass::calculateNormals()
{
	int i, j, index1, index2, index3, index, count;
	float vertex1[3], vertex2[3], vertex3[3], vector1[3], vector2[3], sum[3], length;
	D3DXVECTOR3* normals;


	// Create a temporary array to hold the un-normalized normal vectors.
	normals = new(3) D3DXVECTOR3[(m_terrainHeight - 1) * (m_terrainWidth - 1)];
	if (!normals)
	{
		return false;
	}

	// Go through all the faces in the mesh and calculate their normals.
	for (j = 0; j<(m_terrainHeight - 1); j++)
	{
		for (i = 0; i<(m_terrainWidth - 1); i++)
		{
			index1 = (j * m_terrainHeight) + i;
			index2 = (j * m_terrainHeight) + (i + 1);
			index3 = ((j + 1) * m_terrainHeight) + i;

			// Get three vertices from the face.
			vertex1[0] = m_heightMap[index1].x;
			vertex1[1] = m_heightMap[index1].y;
			vertex1[2] = m_heightMap[index1].z;

			vertex2[0] = m_heightMap[index2].x;
			vertex2[1] = m_heightMap[index2].y;
			vertex2[2] = m_heightMap[index2].z;

			vertex3[0] = m_heightMap[index3].x;
			vertex3[1] = m_heightMap[index3].y;
			vertex3[2] = m_heightMap[index3].z;

			// Calculate the two vectors for this face.
			vector1[0] = vertex1[0] - vertex3[0];
			vector1[1] = vertex1[1] - vertex3[1];
			vector1[2] = vertex1[2] - vertex3[2];
			vector2[0] = vertex3[0] - vertex2[0];
			vector2[1] = vertex3[1] - vertex2[1];
			vector2[2] = vertex3[2] - vertex2[2];

			index = (j * (m_terrainHeight - 1)) + i;

			// Calculate the cross product of those two vectors to get the un-normalized value for this face normal.
			normals[index].x = (vector1[1] * vector2[2]) - (vector1[2] * vector2[1]);
			normals[index].y = (vector1[2] * vector2[0]) - (vector1[0] * vector2[2]);
			normals[index].z = (vector1[0] * vector2[1]) - (vector1[1] * vector2[0]);
		}
	}

	// Now go through all the vertices and take an average of each face normal 	
	// that the vertex touches to get the averaged normal for that vertex.
	for (j = 0; j<m_terrainHeight; j++)
	{
		for (i = 0; i<m_terrainWidth; i++)
		{
			// Initialize the sum.
			sum[0] = 0.0f;
			sum[1] = 0.0f;
			sum[2] = 0.0f;

			// Initialize the count.
			count = 0;

			// Bottom left face.
			if (((i - 1) >= 0) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (m_terrainHeight - 1)) + (i - 1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Bottom right face.
			if ((i < (m_terrainWidth - 1)) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (m_terrainHeight - 1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Upper left face.
			if (((i - 1) >= 0) && (j < (m_terrainHeight - 1)))
			{
				index = (j * (m_terrainHeight - 1)) + (i - 1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Upper right face.
			if ((i < (m_terrainWidth - 1)) && (j < (m_terrainHeight - 1)))
			{
				index = (j * (m_terrainHeight - 1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Take the average of the faces touching this vertex.
			sum[0] = (sum[0] / (float)count);
			sum[1] = (sum[1] / (float)count);
			sum[2] = (sum[2] / (float)count);

			// Calculate the length of this normal.
			length = sqrt((sum[0] * sum[0]) + (sum[1] * sum[1]) + (sum[2] * sum[2]));

			// Get an index to the vertex location in the height map array.
			index = (j * m_terrainHeight) + i;

			// Normalize the final shared normal for this vertex and store it in the height map array.
			m_heightMap[index].nx = (sum[0] / length);
			m_heightMap[index].ny = (sum[1] / length);
			m_heightMap[index].nz = (sum[2] / length);

		}
	}

	return true;
}

void HeightMapLoaderClass::calculateTextureCoordinates()
{
	int incrementCount, i, j, tuCount, tvCount;
	float incrementValue, tuCoordinate, tvCoordinate;


	// Calculate how much to increment the texture coordinates by.
	incrementValue = 1.0f / (float)(m_terrainWidth - 1);


	//разное смещение текстуры в зависимости от уровня

	/*
	switch (lvl)
	{
	case 0: //lvl 1
	//incrementValue *= 1.0068;
	incrementValue *= 1.0149;
	break;
	case 1: //lvl 2
	incrementValue *= 1.0303;
	break;
	case 2: //lvl 3
	incrementValue *= 1.062;
	break;
	case 3: //lvl 4
	incrementValue *= 1.122;
	break;
	default:
	break;
	}
	*/
	// Calculate how many times to repeat the texture.
	incrementCount = m_terrainWidth;

	// Initialize the tu and tv coordinate values.
	tuCoordinate = 0;
	tvCoordinate = 1;

	// Initialize the tu and tv coordinate indexes.
	tuCount = 0;
	tvCount = 0;

	// Loop through the entire height map and calculate the tu and tv texture coordinates for each vertex.
	for (j = 0; j<m_terrainHeight; j++)
	{

		for (i = 0; i<m_terrainWidth; i++)
		{

			// Store the texture coordinate in the height map.
			m_heightMap[(m_terrainHeight * j) + i].tu = tuCoordinate;
			m_heightMap[(m_terrainHeight * j) + i].tv = tvCoordinate;

			// Increment the tu texture coordinate by the increment value and increment the index by one.
			tuCoordinate += incrementValue;
			tuCount++;
		}
		tuCoordinate = 0.00f;

		// Increment the tv texture coordinate by the increment value and increment the index by one.
		tvCoordinate -= incrementValue;

	}

	return;
}
